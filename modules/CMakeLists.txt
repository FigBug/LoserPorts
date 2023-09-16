cmake_minimum_required(VERSION 3.15)

add_subdirectory(juce)
juce_set_vst2_sdk_path(plugin_sdk/vstsdk2.4)

juce_add_module(gin/modules/gin)
juce_add_module(gin/modules/gin_dsp)
juce_add_module(gin/modules/gin_gui)
juce_add_module(gin/modules/gin_plugin)
juce_add_module(gin/modules/gin_graphics)
