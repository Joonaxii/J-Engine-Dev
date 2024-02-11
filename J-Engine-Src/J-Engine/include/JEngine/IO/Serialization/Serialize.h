#pragma once
#include <JEngine/Utility/EnumUtils.h>
#include <JEngine/GUI/Gui.h>

namespace JEngine {
    enum class FieldFlags : uint64_t {
        NONE = 0x00,

        IS_ENUM = 0x01,
        IS_BIT_FIELD = 0x02,

        IS_VECTOR = 0x04,

        ENUM_MASK = IS_ENUM | IS_BIT_FIELD,
        META_DATA_MASK = 0xFFFFFFFF00000000ULL
    };
    CREATE_ENUM_OPERATORS(FieldFlags);

    struct SerializedItem;
    using CustomDraw = bool(*)(SerializedItem& item);

    struct FieldInfo {
        std::string_view name{};
        std::string_view displayName{};

        size_t offset{};
        size_t size{};
        VType type{};
        FieldFlags flags{};
        const GUIStyle* style{};

        struct EnumInfo {
            enum Flags : uint8_t {
                FLAG_NONE = 0x00,
                FLAG_IS_SIGNED = 0x1,
            };

            Flags flags{};
            union Min {
                int64_t i64;
                uint64_t ui64;

                Min() : i64(0) {}
                Min(int64_t value) : i64(value) {};
                Min(uint64_t value) : ui64(value) {};

            } minValue;
            size_t count{};
            Enum::GetEnumName enumNameGet{};     

            EnumInfo() : flags{}, minValue{}, count{}, enumNameGet{ nullptr } {}

            constexpr bool isSigned() const { return (flags & FLAG_IS_SIGNED) != 0; }
        } enumMeta;
  
        FieldInfo() : name{}, offset{}, type{}, displayName{}, flags{}, style{}, enumMeta{}, size{} {}
        FieldInfo(std::string_view name, std::string_view displayName, size_t offset, VType type, size_t size, FieldFlags flags, const GUIStyle& style) :
            name{ name }, displayName{ displayName }, offset{ offset }, type{ type }, flags{ flags }, enumMeta{ }, size{ size }, style{&style}
        {
        }
    };

    struct Type {
        std::string_view name{};
        uint32_t hash{};
        size_t size{};
        std::vector<FieldInfo> fields{};

        CustomDraw customDraw{};

        Type() : name(""), hash(), size(SIZE_MAX), customDraw{nullptr}{}
        Type(std::string_view name, uint32_t hash, size_t size) : name(name), hash(hash), size(size), fields{}, customDraw{nullptr} {}

        template<typename T, size_t ID>
        void addField(std::string_view name, std::string_view displayName, size_t offset, VType type, size_t size, FieldFlags flags, const GUIStyle& style) {
            if (EnumNames<T, ID>::Count < 1 && flags & FieldFlags::IS_ENUM) {
                flags &= ~FieldFlags::ENUM_MASK;
            }
            else if ((flags & FieldFlags::IS_ENUM)) {
                flags = EnumNames<T, ID>::IsBitField ? (flags | FieldFlags::IS_BIT_FIELD) : (flags & ~FieldFlags::IS_BIT_FIELD);
            }

            FieldInfo& field = fields.emplace_back(name, displayName, offset, type, size, flags);
            if (flags & FieldFlags::IS_ENUM) {
                static constexpr bool IS_SIGNED = std::is_signed<BASE_TYPE(T)>::value;
                auto& enumMeta = field.enumMeta;
                enumMeta.enumNameGet = Enum::nameOf_Void<T, ID>;
                enumMeta.flags = IS_SIGNED ? FieldInfo::EnumInfo::FLAG_IS_SIGNED : FieldInfo::EnumInfo::FLAG_NONE;
                enumMeta.minValue = FieldInfo::EnumInfo::Min(EnumNames<T, ID>::MinValue);
                enumMeta.count = EnumNames<T, ID>::Count;
            }
        }
    };

    namespace Types {
        template <typename T>
        constexpr inline std::string_view getTypeName() {
#if defined(__clang__)
            constexpr auto prefix = std::string_view{ "[T = " };
            constexpr auto suffix = "]";
            constexpr auto function = std::string_view{ __PRETTY_FUNCTION__ };
#elif defined(__GNUC__)
            constexpr auto prefix = std::string_view{ "with T = " };
            constexpr auto suffix = "; ";
            constexpr auto function = std::string_view{ __PRETTY_FUNCTION__ };
#elif defined(_MSC_VER)
            constexpr auto prefix = std::string_view{ "getTypeName<" };
            constexpr auto suffix = ">(";
            constexpr auto function = std::string_view{ __FUNCSIG__ };
#else
# error Unsupported compiler
#endif
            const auto start = function.find(prefix) + prefix.size();
            const auto end = function.find(suffix, start);
            const auto size = end - start;
            return function.substr(start, size);
        }

        template<typename T>
        constexpr std::string_view getTypeName(const T& obj) {
            return getTypeName<T>();
        }

        constexpr uint32_t calculateNameHash(std::string_view name) {
            uint32_t hash = Data::calcualteCRC(&name[0], name.length());
            return hash == 0x00 ? 1 : hash;
        }

        template<typename T>
        constexpr inline uint32_t getTypeHash() {
            return calculateNameHash(getTypeName<T>());
        }

        template<typename T>
        inline uint32_t getTypeHash(const T& obj) {
            return calculateNameHash(getTypeName<T>());
        }
    }

    class TypeHelpers {
    public:
        static std::vector<Type*>& getTypes();

        template<typename T>
        static Type const& getType();

        static Type const* getTypeByHash(uint32_t hash) {
            if (hash == 0x00) { return nullptr; }
            auto& types = getTypes();
            for (size_t i = 0; i < types.size(); i++) {
                if (types[i]->hash == hash) { return types[i]; }
            }
            return nullptr;
        }

        static std::string_view getTypeName(uint32_t id) {
            auto type = getTypeByHash(id);
            return type ? type->name : std::string_view();
        }

        template<typename T>
        static uint32_t getTypeHash() {
            Type const* type = &JEngine::TypeHelpers::getType<T>();
            return type ? type->hash : 0x00;
        }
    };
}

#define BUILD_FLAGS(FLAGS, TYPE) ::JEngine::FieldFlags(FLAGS | (uint64_t(::JEngine::Types::getTypeHash<TYPE>()) << 32))

#define ADD_FIELD_0(TYPE, FIELD, F_TYPE, V_TYPE) TYPE.addField<F_TYPE, 0>(#FIELD, #FIELD, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), ::JEngine::FieldFlags::NONE, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_1(TYPE, FIELD, F_TYPE, V_TYPE, D_NAME) TYPE.addField<F_TYPE, 0>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), ::JEngine::FieldFlags::NONE, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_2(TYPE, FIELD, F_TYPE, V_TYPE, D_NAME, FLAGS) TYPE.addField<F_TYPE, 0>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), FLAGS, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_3(TYPE, FIELD, F_TYPE, V_TYPE, D_NAME, FLAGS, STYLE) TYPE.addField<F_TYPE, 0>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), FLAGS, STYLE)

#define ADD_FIELD_0_ID(ID, TYPE, FIELD, F_TYPE, V_TYPE) TYPE.addField<F_TYPE, ID>(#FIELD, #FIELD, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), ::JEngine::FieldFlags::NONE, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_1_ID(ID, TYPE, FIELD, F_TYPE, V_TYPE, D_NAME) TYPE.addField<F_TYPE, ID>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), ::JEngine::FieldFlags::NONE, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_2_ID(ID, TYPE, FIELD, F_TYPE, V_TYPE, D_NAME, FLAGS) TYPE.addField<F_TYPE, ID>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), FLAGS, ::JEngine::Gui::Styles::Default)
#define ADD_FIELD_3_ID(ID, TYPE, FIELD, F_TYPE, V_TYPE, D_NAME, FLAGS, STYLE) TYPE.addField<F_TYPE, ID>(#FIELD, D_NAME, offsetof(F_TYPE, FIELD), V_TYPE, sizeof(F_TYPE::FIELD), FLAGS, STYLE)

#define XXX_X(x,A,B,C,D,E,F,G,FUNC, ...)  FUNC  

#define ADD_FIELD(...) XXX_X(,##__VA_ARGS__,\
                                         ADD_FIELD_3(__VA_ARGS__),\
                                         ADD_FIELD_2(__VA_ARGS__),\
                                         ADD_FIELD_1(__VA_ARGS__),\
                                         ADD_FIELD_0(__VA_ARGS__),\
                                           ,\
                                           ,\
                                           \
                                         ) 

#define ADD_FIELD_ID(...) XXX_X(,##__VA_ARGS__,\
                                         ADD_FIELD_3_ID(__VA_ARGS__),\
                                         ADD_FIELD_2_ID(__VA_ARGS__),\
                                         ADD_FIELD_1_ID(__VA_ARGS__),\
                                         ADD_FIELD_0_ID(__VA_ARGS__),\
                                           ,\
                                           ,\
                                           \
                                         ) 

/*
    ............',,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,''''''''''',,'',,,,,''''''''''''''''''''''',,,,,,,,,,,;;;::::;;;,,,'''...
  ..............,,,,,,;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,''''''''''''''',,,,,,,,,,;;;::::;;;,,,,''..'
 ..............',,,,;;;;,,,,;;;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,''''''''',,,,,,,;;;:::::;;;;,,,,,,'''
...............',,,;;;;,,;;;;::;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,',,,,,''',,,,,;;;;;;::::;;;;,,,,,,,''
...............,,;;;;;,;;;:::::;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;;:::::::;;;;;;,,,;,,''
..............',;;;;;;;;::::::;;;;;;;,,,,,,,,,;,,,;;;;;;;;;;;,,,,,,,,,;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;;:::::::;;;;;;;;;;,,''
..............',;;;;;;:::cc:::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,,,,,;;;;;;::::;;;;;;;;;;;;;,,''
..............,;;;;;;:cccc:::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,;;::::;:::;;;;;;;;;;;;;,'''
..............,;;;;::ccccc::;;;,;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,;;;::c::;::;;;;;;;;;;;:;,'''
 .... .     .',;;::cccccc:;;;;,,,;;;;;:::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,;;;;;:::::::::;;;;;;;;;::;''''
  ...  .    .';;:cccccc::;;;;;;,,;;;;;:::::::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,;;;;;;;:::::::;;;;;;;;::::,''''
      .... ..,;:cclcc::;;;;;;;;,,,;;;;::::::::::::::::;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,;;;;;:::::::::::;;;;::::::;'''''
    ........';ccccc::;;;;;;;;,,,,,,;;;:::::::::::::::::::;;;::::::::::::::::;;;;;;;;;;;;;;;;;;;;;;;;::::::::::::;;::::::c:,'''.'
    ........;::::::;;;;;;;;;,,,,,,,,;;;:::::::::::::::::::::::::::::::::::::::::::;;;;;;;;;;;;;;;;;;::::::::::::;;::::::c;'..'.'
    .......,::;;;;;;;;;;;,,;,,,,,,,,;;;;:::::::::::::::::;;;;;;;;;::::::ccccccccc::::;;;;;;;;;;;;;::::::c::::::::::::::c:,.....'
  ........,;;;;;;;;;;;;;;;,,,,,''',,,;;;;:::::::::::::::;;;;;;;;;;;;:::::ccccccccccc:::::;;;;::::::::::cc:::::::;:::::cc,......'
..........;;;;;;;;;;;;;,,,,''.....',,,;,,;::::::::::::::;;;;;;;;;;;;;;:::::ccccccccccc:::::::::::::::ccccc:::::::::::cc;'.......
.........';;;;;,,,,,,,''..........',,,;;'.';::::::::::;;;;;;;;;;;;;;;;;:::::::ccccccccccc::::cccccccccccccc:::::::::cc:'........
...........''''''........    ....',,,,,;,...,;;;::::;;;;;;;;;;;;;;;;;;;;:::::::::ccccccccccccccccccccccccc:::::::::cc:,.........
.....................      ....'',,;,,,;;,....',;;;;;;;;;;;;;;;;;;;;;;;;:::::::::::cccccccccccccccccccccc:::::::::cc:,..........
.........               .....'',,,;;;,,;;;,'.....',,;;;;;;;;;;;;;;;;;;;;;;;;;;;;;:::::ccccccccccccccccc:::::;;:::cc:,...........
....  ...         .......''',,,;;;;;;;,,;;;;,'........''',,,,,,,'''',,,,,,,;;;;;;;;;:::ccccccccllccccc:::;;;;;::cc:,............
..........     ...'''',,,,,,,,;;;;;;;;;,;;;,;;,'...........................''',,,;;;;:::ccclllllllccc:::;;;;;:::c:'.........  ..
............  ..',,,,,,,,,,,,,,,,;;;;;;;;;;,,;;;,,....                ..........',,;;;::ccclllllllcc:::;;;;;::::;'........  ....
................',,,,,,,,,,,,,,,,,;;;;;;;;;;,,,,;;,,'.....                ........',,;;::cclllllllc:::;;;;;;:::,.......   ......
..................',;,,,,,,,,,,,,,,,;;;;;;;;,,,,,;;;;;,,''.....              ......',,;::ccllllllcc:;;;;;;;::;'......  .........
....................',,,,,,,,,,,,,,,,,,,;;;;,,,,,,,,;;,,,,,,,'''.......        .....',;;:cclllllcc:;;;;;;;:;'......  ...........
.......................,,,,,,,'''',,,,,,;;;;,,,,,,,,,,,,,,,,,,,,,,,,,,''...      ....',;:cclllllc:;;,,;;;;,.....   .............
.........................,,,'.....'''',,,;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,'...... ....';:ccllllc:;;,,,,,'.....   ...............
....................... ........  ....'',;;;,,,,,,,,,,,,,,',,,,,,,,,,,,,,,,,,'''......',:cclllc:;;,,,......    .................
...............            .;:;'.     ..';;;,,',,,,,,'''''''''''''''',,,,,,,,,,,,''....,:clllc::;,'.          ..................
...........               .cdo:'..      .,;;,,'''''''''''..........''''',,,,,,,,,,,''..,;clllc:;;..  ....   ....................
..........               .;ddlc,.   .'. .,;;,,''''''''....        .....''',,,,,,,,,,,'',:cllc:::;...,;:;'. .....................
........                 .:ddc,.    .,'  ';;,'''''''..........         ...'',,,,,,,,,,,;:cllc:,'''',,,'.. ......................
......                   .;ooc...        ,;;,,'''''....,:c:,..           ..'''',,,,,,,;:cllc;'...'.....     ....................
....                    .'.,cc,....     .;;,,,,'''....,cloc...   ..        ..''''''',,;cllc;'.',,'....       ...................
                       .;;'..'''........,:;,,',,''...':loc,.   .',,.    ..  ..','''',;:llc;'.''''...          ..................
                       .;;,''..........,::;,,,,,''...,cll:.    .'''.   .''. .',;;;;;:cllc,........              ................
                       .;:;,,'......',;::;;,,,,'''...':ll:..           .::,..,:cccclllol:'.'''..                . ..............
                       .;:;;;,,,,,;;::cc:;,,''''''....,clc;......... ..;c:,',:clloooool:;,''..                    .. ...........
    ...                .,:;;;;;;;:::ccc:;,'''''''......',::;'.........,,'.';clloooooooc;'..                         ............
..   ........       ....';;;;;;;::ccc:;;,'''''''''....................''',;:cloooooool,.                              ..........
       ..................,;;;;;::ccc:;,,''''',,,,''..................'',;;:clloooooo:.                                   ... ...
         ................';,,;:cccc:;,,,''''',,,,,,'''''...........'',,;::cclllllll;.                                      ... .
              .....  .  .';;;:cccc:;,,,,'''',,,,,,,,,,,,,,''''''',,,;;;;:::ccccccc,. ...                                      ..
                ..       .;::cccc:;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;;;;;;:::::,........
                ....... .,:ccccc:;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,;;;;::;...........
                ........,:ccccc::::::::::::::;;;,,,,,,,,,,,,,,,,,'''',,,,,,;;:::,..  ..........      ...'.
                .......':ccccc::::cccccllcccc:;;;;;,,,,,,,,,,''''''',,,,,;;;:c:,;:'............... ....,l,
                .......;cccccc:cclllloooollcc:::::;;,'''',,,,,''''',,,,,;;::c:..':;,'................ .;:.
                ......,:ccccccclloooodddoollcccc:::;,''''',,,,,,,,,,,,,;;::c:'  .''''''..........    ..,..
                 ....':clllcclloodddddddoollccccc:;,''''''',,,,,,,,,,;;;::c:'   ..........................
                  ..':cllllllooddddxxxddollllcc::;,'''''''',,,,,,,,,;;::ccc'      .  .......................
                 ..;:clllooooddddddddddollccc::;,'''''''''',,,,,,,;;::cccc,            .......................
                .':ccllloooodddddddddoolc::;;;;,,,,,''''''',,,,,;;;::cccc,.               .......................
                 .,;::cllooodddddooollc:;;;:::::::;;,,''''',,,,;;::ccccc'                            ..............
                  .,;;;:clllllllllcc:::::cclllllccc:;;,,'''',,;;::cccc:'                               .............
                 ...',,,;:::ccc::;;;::ccllooooooollcc:;,,''',;;:::ccc:'                                    ..........
             .   .....''',,;;,,,,,,;;::cclllllllooollc:;,'',,;:::ccc:.                                        ........
            ..        ....',,''......',,;;:::;;;;:cllc:;,'',,;::ccc;.
            .            .',,........'',,;:cclcc;,,,:cc;,'',;;::c:,.
          ..             .',,..  .....NO   FIELDS?:;,,;;,'',;::;;,..   .
                        ..','..    ....''''',,,;;::::,,,,',;;:;'.........
  ..                   ...''...    ......'''''',,,;;;;'''',,;,'.........
*/
#define NO_FIELDS static void bindFields(::JEngine::Type& type) { }

#define DEFINE_TYPE(TYPE)\
template<> inline ::JEngine::Type const& ::JEngine::TypeHelpers::getType<TYPE>() { \
        static bool init{ false }; \
        static ::JEngine::Type type( \
            ::JEngine::Types::getTypeName<TYPE>(), \
            ::JEngine::Types::getTypeHash<TYPE>(), sizeof(TYPE)); \
    if (!init) { \
        TYPE::bindFields(type); \
        JEngine::TypeHelpers::getTypes().push_back(&type); \
        init = true; \
    } \
    return type; \
}
