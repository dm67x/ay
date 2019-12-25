cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE MODEL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/models/**)
file(GLOB_RECURSE SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/**)

message("Copying model files")
file(COPY ${MODEL_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/models)
message("Copying shader files")
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders)