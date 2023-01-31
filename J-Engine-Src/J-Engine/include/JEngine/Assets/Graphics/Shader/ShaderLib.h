#pragma once
#include <string>
#include <JEngine/Assets/IAsset.h>

namespace JEngine {
    class ShaderLib : public IAsset {
    public:
        ShaderLib();

        void parse(const std::string& filepath);
        void parse(std::istream& stream);

        void clear();

    private:
        enum FuncDefType : uint8_t {
            FUNC_Unknown,

            //Misc
            FUNC_Void,
            FUNC_Bool,
            FUNC_Uniform,

            //Floating-point Scalars
            FUNC_Float,
            FUNC_Double,

            //Integer Scalars
            FUNC_Int,
            FUNC_UInt,

            //Int Vectors
            FUNC_IVec2,
            FUNC_IVec3,
            FUNC_IVec4,

            //UInt Vectors
            FUNC_UVec2,
            FUNC_UVec3,
            FUNC_UVec4,

            //Vectors
            FUNC_Vec2,
            FUNC_Vec3,
            FUNC_Vec4,

            //Double Vectors
            FUNC_DVec2,
            FUNC_DVec3,
            FUNC_DVec4,

            //Matrices
            FUNC_Mat2,
            FUNC_Mat3,
            FUNC_Mat4,

            FUNC_Mat2x3,
            FUNC_Mat2x4,

            FUNC_Mat3x2,
            FUNC_Mat3x4,

            FUNC_Mat4x2,
            FUNC_Mat4x3,

            //Double Matrices
            FUNC_DMat2,
            FUNC_DMat3,
            FUNC_DMat4,

            FUNC_DMat2x3,
            FUNC_DMat2x4,

            FUNC_DMat3x2,
            FUNC_DMat3x4,

            FUNC_DMat4x2,
            FUNC_DMat4x3,

            //Samplers
            FUNC_Sampler2D,
            FUNC_Sampler1D,

            //Structs
            FUNC_Struct,
        };

        struct GLStruct {
            std::string name = "";
            std::string code = "";
        };

        struct FunctionDefinition {
            static constexpr uint8_t MAX_PARAMS = 32;

            FuncDefType returnType = FuncDefType::FUNC_Void;

            uint8_t paramCount = 0;
            FuncDefType params[MAX_PARAMS]{ FuncDefType::FUNC_Unknown };

            std::string code = "";
        };

        static const FuncDefType strToFuncDef(const char* str, const bool isArg = false) {
            static struct cmp_str
            {
                bool operator()(char const* a, char const* b) const { std::strcmp(a, b) < 0; }
            };

            static std::map<const char*, uint8_t, cmp_str> localMap;
            if (localMap.empty())
            {
                localMap["uniform"]     = FUNC_Uniform;
                localMap["void"]        = FUNC_Void;
                localMap["bool"]        = FUNC_Bool;

                localMap["sampler1D"]   = FUNC_Sampler1D;
                localMap["sampler2D"]   = FUNC_Sampler2D;
                localMap["struct"]      = FUNC_Struct;
            }

        }

        std::vector<GLStruct> _structs;
        std::vector<ShaderLib*> _dependencies;
    };
}