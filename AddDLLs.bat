echo %1
echo %2
xcopy %1assimp\bin\x64\assimp-vc143-mt.dll %2 /Y
xcopy %1assimp\bin\x64\D3DCompiler_42.dll %2 /Y
xcopy %1assimp\bin\x64\D3DX9_42.dll %2 /Y