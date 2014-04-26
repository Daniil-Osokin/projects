from ironpywinauto.control_specification import ControlSpecification

poc_spec = ControlSpecification(None, None, "GUIAT - Proof of Concept", None, 'frmGUIAT', None, None, None, True)
#run GUIAT_PoC.exe (or don't for timeout error)
print poc_spec.Wait('exists', 10).Name
