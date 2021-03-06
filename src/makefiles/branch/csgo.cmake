# ------------------------------------------------------------------
# File: src/makefiles/game/csgo.cmake
# Purpose: This is the CMake file for CSGO.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Includes.
# ------------------------------------------------------------------
include("makefiles/branch/${BRANCH}/usermessages.cmake")

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "csgo")
add_definitions(-DENGINE_CSGO)