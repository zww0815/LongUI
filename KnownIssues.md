﻿### Known Issues
  
1. Memory leak: under **not** WARP-Adapter, memory leak detected by `ID3D11Debug` at end of program  
    - **guess**: maybe microsoft bug? or intention for optimization?
    - **solve**: 2015/09/07- yeah! bug for MS, MS fixed it!
2. Meta rendering: button-like Meta rendering look like in wrong way  
    - **guess**: maybe wrong way with float ceil-floor?
    - **solve**: none yet
