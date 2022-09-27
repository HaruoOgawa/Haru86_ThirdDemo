set EnvPath=%~dp0

set EnvPathDebug=%EnvPath%x64\Debug\bin
set EnvPathRelease=%EnvPath%x64\Release\bin
set EnvPathDemoRelease=%EnvPath%x64\DemoRelease\bin

setx /M Path "%Path%;%EnvPathDebug%;%EnvPathRelease%;%EnvPathDemoRelease%"

@rem echo %EnvPathDebug%
@rem echo %EnvPathRelease% 
@rem echo %EnvPathDemoRelease%
 
@rem pause