# ../core/__init__.py

"""Provides core functionality that doesn't fit into any other package."""

# =============================================================================
# >> IMPORTS
# =============================================================================
# Python Imports
#   Inspect
from inspect import getmodule
from inspect import stack
#   OS
from os import sep
#   Platform
from platform import system
#   Sys
import sys

# Source.Python Imports
#   Loggers
from loggers import _sp_logger
#   Paths
from paths import GAME_PATH


# =============================================================================
# >> FORWARD IMPORTS
# =============================================================================
# Source.Python Imports
#   Constants
from constants import SOURCE_ENGINE
#   Globals
from _globals import global_vars
from _globals import MapLoadType


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
__all__ = ('AutoUnload',
           'GAME_NAME',
           'MapLoadType',
           'PLATFORM',
           'SOURCE_ENGINE',
           'echo_console',
           'global_vars',
           )


# =============================================================================
# >> GLOBAL VARIABLES
# =============================================================================
# Get the specific game for the server
GAME_NAME = GAME_PATH.rsplit(sep, 2)[1]

# Get the platform the server is on
PLATFORM = system().lower()

# Get the sp.core logger
core_logger = _sp_logger.core


# =============================================================================
# >> CLASSES
# =============================================================================
class AutoUnload(object):

    """Class used to auto unload specific instances.

    Each inheriting class must implement an _unload_instance method.
    """

    def __new__(cls, *args, **kwargs):
        """Overwrite __new__ to store the calling module."""
        # Get the class instance
        self = super(AutoUnload, cls).__new__(cls)

        # Get the calling module
        caller = getmodule(stack()[1][0])

        # Set the _calling_module attribute for the instance
        self._calling_module = caller.__name__

        # Return the instance
        return self

    def _unload_instance(self):
        """Base _unload_instance implementation."""
        raise NotImplementedError(
            'Class "{0}" from file "{1}" does not '.format(
                self.__class__.__name__, sys.modules[
                    self.__class__.__module__].__file__.split(
                    'plugins', 1)[1][1:]) +
            'have its own implementation of an _unload_instance method.')


# =============================================================================
# >> FUNCTIONS
# =============================================================================
def echo_console(text):
    """Echo a message to the server's console."""
    # Import engine_server
    # This is done here to fix an ImportError
    from engines.server import engine_server

    # Loop through each line in the text
    for line in text.split('\n'):

        # Echo the message
        engine_server.server_command(
            'echo "{0}"\n'.format(line.replace('"', "'")))
