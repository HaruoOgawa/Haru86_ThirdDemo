set EnvPath=%~dp0

set EnvPathDebug=%EnvPath%Debug\bin
set EnvPathRelease=%EnvPath%Release\bin
set EnvPathDemoRelease=%EnvPath%DemoRelease\bin
set Path_setup_x64=%EnvPath%setup_x64.bat

setx /M Path "%Path%;%EnvPathDebug%;%EnvPathRelease%;%EnvPathDemoRelease%"

attrib +s +h %EnvPathDebug%
attrib +s +h %EnvPathDebug%
attrib +s +h %EnvPathDebug%
attrib +s +h %Path_setup_x64%

@rem echo %EnvPathDebug%
@rem echo %EnvPathRelease% 
@rem echo %EnvPathDemoRelease%
 
@rem pause