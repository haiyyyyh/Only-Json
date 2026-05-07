#include <print>
import json;

using namespace hai;

int main(){
    json j = R"(
        {
            "BreakBeforeTemplateCloser": {
                "description": "clang-format 21\n\nIf true, break before a template closing bracket (>) when there is a line break after the matching opening bracket (<).",
                "type": "boolean"
            },
            "BreakBinaryOperations": {
                "description": "clang-format 20\n\nThe break binary operations style to use.",
                "enum": ["Never", "OnePerLine", "RespectPrecedence"],
                "type": "string"
            },
            "BreakFunctionDefinitionParameters": {
                "description": "clang-format 19\n\nIf true, clang-format will always break before function definition parameters.",
                "type": "boolean"
            },
            "BreakTemplateDeclarations": {
                "description": "clang-format 19\n\nThe template declaration breaking style to use.",
                "enum": ["Leave", "No", "MultiLine", "Yes"],
                "type": "string"
            }
        }
    )"_json;
    std::print("{}", j.dump());
    return 0;
}