"""
Python package for automating GUI manipulation on Windows

"""
from __future__ import absolute_import

__revision__ = "$Revision$"
__version__ = "0.0.1"

#=========================================================================
class WindowNotFoundError(Exception):
    "No window could be found"
    pass

#=========================================================================
class WindowAmbiguousError(Exception):
    "There was more then one window that matched"
    pass

#=========================================================================
class TimeoutError(Exception):
    "Time limit is reached"
    pass
