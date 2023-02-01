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

        static const FuncDefType strToDataType(const char* str, const bool isArg = false) {
            static struct cmp_str
            {
                bool operator()(char const* a, char const* b) const { return (std::strcmp(a, b) < 0); }
            };

            static std::map<const char*, uint8_t, cmp_str> localMap;
            if (localMap.empty())
            {
                //Misc
                localMap["void"]        = FUNC_Void;
                localMap["bool"]        = FUNC_Bool;
                localMap["uniform"]     = FUNC_Uniform;

                //Scalars
                localMap["float"]       = FUNC_Float;
                localMap["double"]      = FUNC_Double;

                localMap["int"]         = FUNC_Int;
                localMap["uint"]        = FUNC_UInt;

                //Vectors
                localMap["ivec2"]       = FUNC_IVec2;
                localMap["ivec3"]       = FUNC_IVec3;
                localMap["ivec4"]       = FUNC_IVec4;

                localMap["uvec2"]       = FUNC_UVec2;
                localMap["uvec3"]       = FUNC_UVec3;
                localMap["uvec4"]       = FUNC_UVec4;    
                
                localMap["vec2"]        = FUNC_Vec2;
                localMap["vec3"]        = FUNC_Vec3;
                localMap["vec4"]        = FUNC_Vec4;

                localMap["dvec2"]       = FUNC_DVec2;
                localMap["dvec3"]       = FUNC_DVec3;
                localMap["dvec4"]       = FUNC_DVec4;

                //Matrices
                localMap["mat2"]        = FUNC_Mat2;
                localMap["mat3"]        = FUNC_Mat3;
                localMap["mat4"]        = FUNC_Mat4;

                localMap["mat2x2"]      = FUNC_Mat2;
                localMap["mat3x3"]      = FUNC_Mat3;
                localMap["mat4x4"]      = FUNC_Mat4;

                localMap["mat2x3"]      = FUNC_Mat2x3;
                localMap["mat2x4"]      = FUNC_Mat2x4;
   
                localMap["mat3x2"]      = FUNC_Mat3x2;
                localMap["mat3x4"]      = FUNC_Mat3x4;

                localMap["mat4x2"]      = FUNC_Mat4x2;
                localMap["mat4x3"]      = FUNC_Mat4x3;

                localMap["dmat2"]       = FUNC_DMat2;
                localMap["dmat3"]       = FUNC_DMat3;
                localMap["dmat4"]       = FUNC_DMat4;

                localMap["dmat2x2"]     = FUNC_DMat2;
                localMap["dmat3x3"]     = FUNC_DMat3;
                localMap["dmat4x4"]     = FUNC_DMat4;

                localMap["dmat2x3"]     = FUNC_DMat2x3;
                localMap["dmat2x4"]     = FUNC_DMat2x4;

                localMap["dmat3x2"]     = FUNC_DMat3x2;
                localMap["dmat3x4"]     = FUNC_DMat3x4;

                localMap["dmat4x2"]     = FUNC_DMat4x2;
                localMap["dmat4x3"]     = FUNC_DMat4x3;

                //Samplers
                localMap["sampler1D"]   = FUNC_Sampler1D;
                localMap["sampler2D"]   = FUNC_Sampler2D;

                //Struct
                localMap["struct"]      = FUNC_Struct;
            }

        }

        std::vector<GLStruct> _structs;
        std::vector<ShaderLib*> _dependencies;
    };
}