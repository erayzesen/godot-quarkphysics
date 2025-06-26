#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

env.Append(CPPPATH=["src/"])
env.Append(CPPPATH=["src/QuarkPhysics/"])
env.Append(CPPPATH=["src/QuarkPhysics/extensions/"])
env.Append(CPPPATH=["src/QuarkPhysics/json/"])
env.Append(CPPPATH=["src/QuarkPhysics/polypartition/"])
#env.Append(CPPPATH=[env.Dir(d) for d in source_path])
sources = [ Glob("src/*.cpp"),Glob("src/QuarkPhysics/*.cpp"),Glob("src/QuarkPhysics/extensions/*.cpp"),Glob("src/QuarkPhysics/json/*.cpp"),Glob("src/QuarkPhysics/polypartition/*.cpp") ]
#sources = Glob("src/*.cpp")

#Native Documentation Section
if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")
#End of the Native Documentation Section

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "project/addons/quarkphysics/bin/libquarkphysics.{}.{}.framework/libquarkphysics.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "project/addons/quarkphysics/bin/libquarkphysics.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "project/addons/quarkphysics/bin/libquarkphysics.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "project/addons/quarkphysics/bin/libquarkphysics{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
