import time
from . import WindowNotFoundError, WindowAmbiguousError, TimeoutError
from .find_control import find_control

class ControlSpecification(dict):

    def __init__(self,
                 title=None,
                 title_re=None,
                 class_name=None,
                 class_name_re=None,
                 auto_id=None,
                 control_type=None,
                 visible=True,
                 enabled=True,
                 top_level_only=False):
        self['title'] = title
        self['title_re'] = title_re
        self['class_name'] = class_name
        self['class_name_re'] = class_name_re
        self['auto_id'] = auto_id
        self['control_type'] = control_type
        self['visible'] = visible
        self['enabled'] = enabled
        self['top_level_only'] = top_level_only
        self.def_timeout = 1

        def IsInState(wait_for, ctl):
            is_passed = True
            if 'exists' in wait_for:
                if not ctl:
                    is_passed = False
            if 'visible' in wait_for:
                if not ctl or ctl.GetSupportedProperties()['IsOffscreen']:
                    is_passed = False
            if 'enabled' in wait_for:
                if not ctl or not ctl.IsEnabled:
                    is_passed = False
            if is_passed:
                return ctl
            return is_passed
        self.IsInStateCallback = IsInState

        def IsNotInState(wait_for, ctl):
            is_passed = True
            try:
                if 'exists' in wait_for:
                    if ctl and ctl.IsEnabled:
                        is_passed = False
                if 'visible' in wait_for:
                    if ctl and not ctl.GetSupportedProperties()['IsOffscreen']:
                        is_passed = False
                if 'enabled' in wait_for:
                    if ctl and ctl.IsEnabled:
                        is_passed = False
            except SystemError: #control was closed
                return True
            return is_passed
        self.IsNotInStateCallback = IsNotInState

    def __WaitWithCallback(self, wait_for, timeout, retry_interval, callback):
        if not timeout:
            timeout = self.def_timeout
        if not retry_interval:
            retry_interval = timeout * 0.1
        start = time.time()
        time_left = timeout - (time.time() - start)
        ctl = None
        wait_for = wait_for.lower()
        if 'ready' in wait_for:
            wait_for = 'visible enabled'
        while (time_left > 0):
            try:
                if not ctl:
                    ctl = find_control(self['title'], self['title_re'], self['class_name'],
                        self['class_name_re'], self['top_level_only'], self['visible'],
                        self['enabled'], self['auto_id'], self['control_type'])
            except WindowAmbiguousError:
                raise WindowAmbiguousError()
            except WindowNotFoundError:
                pass

            res = callback(wait_for, ctl)
            if res:
                return res

            time.sleep(min(retry_interval, time_left))
            time_left = timeout - (time.time() - start)
        raise TimeoutError()

    def Wait(self, wait_for, timeout=None, retry_interval=None):
        return self.__WaitWithCallback(wait_for, timeout, retry_interval, self.IsInStateCallback)

    def WaitNot(self, wait_for, timeout=None, retry_interval=None):
        return self.__WaitWithCallback(wait_for, timeout, retry_interval, self.IsNotInStateCallback)
