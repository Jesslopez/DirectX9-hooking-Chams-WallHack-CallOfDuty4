Project Overview:

Project Title: DirectX 9 Hook for Texture Manipulation and Z-Buffer Management

Description: This project is a research-oriented exploration of DirectX 9 hooking techniques with a focus on manipulating textures and disabling the Z-buffer for specific in-game objects. The primary goal of this project is to study the effects of texture manipulation and Z-buffer management in real-time rendering pipelines by injecting custom rendering code into DirectX-based applications.

Key functionalities of the project include:

DirectX 9 Hooking: Intercept DirectX 9 functions to manipulate game graphics in real-time.
Texture Manipulation: Identify and replace textures for specific game objects using hooked EndScene and other rendering calls.
Z-buffer Control: Disable or adjust Z-buffer settings for certain objects, allowing custom rendering logic to bypass depth testing.
Shader Integration: Optional integration of custom shaders to explore further rendering effects.
Educational Focus: The project is intended as a learning and research tool for those interested in understanding game hacking, rendering pipeline manipulation, and DirectX internals.

Technologies Used:
C++
DirectX 9 SDK
Hooking Techniques (DLL Injection, IAT/EAT modifications)
Reverse Engineering Tools (x64dbg, Cheat Engine)

How to Use:
Clone the repository.
Compile the project using MVSC with appropriate DirectX 9 SDK installed.
Inject the DLL into a target DirectX 9 application.
Customize the hook logic for texture manipulation and Z-buffer control as required.

Disclaimer:
This project is intended solely for educational and research purposes. It should not be used for any malicious activity or to violate any terms of service of third-party applications or games. The author of this project does not endorse or support illegal activities, including unauthorized modification of software. Any misuse of the code provided is solely the responsibility of the user.

Results:
![pic1](https://github.com/user-attachments/assets/0395e883-f691-402b-8f71-6e96de1b3058)
![pic2](https://github.com/user-attachments/assets/8ad66f34-e5cf-4c2f-bdfc-a64e539ff4f4)
![pic3](https://github.com/user-attachments/assets/f4a7bc16-de86-4d85-ba3d-8a7bfcc41784)
![pic4](https://github.com/user-attachments/assets/79452718-1e14-4756-ad5e-8c3e7c98d968)
![pic5](https://github.com/user-attachments/assets/9956b6f7-5195-4ea9-b018-2b4e02d5b580)
