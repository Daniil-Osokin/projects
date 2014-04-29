import re, clr
clr.AddReference('UIAutomationClient')
clr.AddReference('UIAutomationTypes')
from System.Windows.Automation import AutomationElement, TreeScope, PropertyCondition
from . import WindowNotFoundError, WindowAmbiguousError
from .automation_element import PythonicAutomationElement

#Fix for bug with handling ArrayList
clr.AddReferenceToFile('asm/uiauto')
import iprcs

def find_control(class_name = None,
                 class_name_re = None,
                 title = None,
                 title_re = None,
                 top_level_only = False,
                 visible_only = True,
                 enabled_only = False,
                 auto_id = None,
                 control_type = None
    ):
    """Find control based on criteria passed in

    Possible values are:

    * **class_name**  Control of this class name
    * **class_name_re**  Control whose class name match this regular expression
    * **title**     Control with this Title
    * **title_re**  Control whose Title match this regular expression
    * **top_level_only** Top level control only (default=False)
    * **visible_only**   Visible control only (default=True)
    * **enabled_only**   Enabled control only (default=False)
    * **auto_id**  Automation id (default=None)
    * **control_type**  Control type (default=None)
   """

    def find_controls(controls,
   	                  class_name = None,
                      class_name_re = None,
                      title = None,
                      title_re = None,
                      visible_only = True,
                      enabled_only = False,
                      auto_id = None,
                      control_type = None
    ):
        matched_controls = []
        for ctl in controls:
            matched_controls.append(ctl)

        if class_name and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if class_name == ctl.ClassName]

        if class_name_re and matched_controls:
            class_name_regex = re.compile(class_name_re)
            matched_controls = [ctl for ctl in matched_controls
                if class_name_regex.match(ctlClassName)]

        if title and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if title == ctl.Name]

        if title_re and matched_controls:
            title_regex = re.compile(title_re)
            matched_controls = [ctl for ctl in matched_controls
                if title_regex.match(ctl.Name)]

        if visible_only and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if not ctl.GetSupportedProperties()['IsOffscreen']]

        if enabled_only and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if ctl.IsEnabled]

        if auto_id and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if auto_id == ctl.AutomationId]

        if control_type and matched_controls:
            matched_controls = [ctl for ctl in matched_controls
                if control_type == ctl.GetSupportedProperties()['ControlType']]

        return matched_controls

    rootElement = PythonicAutomationElement(iprcs.uiauto().RootElement())
    matched_controls = []
    controls = rootElement.FindAll(TreeScope.Children,
        PropertyCondition(AutomationElement.IsControlElementProperty, True))
    matched_controls = find_controls(controls, class_name, class_name_re,
            title, title_re, visible_only, enabled_only, auto_id, control_type)
    if not top_level_only:
        for control in controls:
            childrens = control.FindAll(TreeScope.Children,
                PropertyCondition(AutomationElement.IsControlElementProperty, True))
            matched_controls = matched_controls + find_controls(childrens, class_name, class_name_re,
                title, title_re, visible_only, enabled_only, auto_id, control_type)

    if (len(matched_controls) == 1):
      return matched_controls[0]
    elif (len(matched_controls) > 1):
      raise WindowAmbiguousError()
    else:
      raise WindowNotFoundError()
