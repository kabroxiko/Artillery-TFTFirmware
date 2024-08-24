import re

Import("env")

build_flags = env.ParseFlags(env['BUILD_FLAGS'])
flags = {k: v for (k, v) in build_flags.get("CPPDEFINES")}
short_version = flags.get("SOFTWARE_VERSION").split('.')[0]
print(f"Short version: {short_version}")
env.Append(BUILD_FLAGS=[f'-DSOFTWARE_VERSION_SHORT={short_version}'])
