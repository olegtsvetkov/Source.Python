# ../downloads.py

# =============================================================================
# >> IMPORTS
# =============================================================================
# Source.Python Imports
from Source import Misc
from engine_c import EngineServer
from core import AutoUnload
#   Events
from events.manager import EventRegistry


# =============================================================================
# >> ALL DECLARATION
# =============================================================================
# Add all the global variables to __all__
__all__ = [
    'Downloadables',
]


# =============================================================================
# >> CLASSES
# =============================================================================
class Downloadables(AutoUnload, set):
    '''Class used to store downloadables for a script'''

    def __init__(self):
        '''Add the instance to the downloadables list'''
        _DownloadablesListInstance.append(self)

    def add(self, item):
        '''Adds an item to the downloadables for a script'''

        # Is the item already in the list?
        if item in self:

            # If so, no need to add it
            return

        # Add the item to the downloadables stringtable
        _DownloadablesListInstance._add_to_download_table(item)

        # Add the item to the script's downloadables
        super(Downloadables, self).add(item)

    def _set_all_downloads(self):
        '''Adds all downloadables for the script on level init'''

        # Loop through all items in the list
        for item in self:

            # Add the item to the downloadables stringtable
            _DownloadablesListInstance._add_to_download_table(item)

    def _unload_instance(self):
        '''Removes the instance from the downloadables list'''
        _DownloadablesListInstance.remove(self)


class _DownloadablesList(list):
    '''List object used to store downloads on a per-script basis'''

    def __init__(self):
        '''Refresh the downloadables table instance'''
        self._refresh_table_instance()

    def _refresh_table_instance(self):
        '''Gets the current instance of the downloadables table'''
        self.download_table = Misc.GetStringTables().FindTable('downloadables')

    def _add_to_download_table(self, item):
        '''Add the given file to the downloadables table'''

        # Lock the network string tables
        locked = EngineServer.lock_network_string_tables(False)

        # Add the given file
        self.download_table.AddString(True, item)

        # Reset the lock status
        EngineServer.lock_network_string_tables(locked)

    def server_spawn(self, GameEvent):
        '''Adds all items stored as downloadables to the stringtable'''

        # Refresh the downloadables table instance
        self._refresh_table_instance()

        # Loop through all items in the list
        for item in self:

            # Set all items in the current item as downloadables
            item._set_all_downloads()

# Get the _DownloadablesList instance
_DownloadablesListInstance = _DownloadablesList()

# Register for the event server_spawn in
# order to reset all downloads on map change
EventRegistry.register_for_event(
    'server_spawn', _DownloadablesListInstance.server_spawn)
