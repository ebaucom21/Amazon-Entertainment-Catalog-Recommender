<h1 align="center" id="title">Amazon Entertainment Catalog Recommender</h1>

<p id="description">A recommendation engine for finding a new show on Amazon prime based on a short questionnaire. This Recommendation engine will employ the use of both a Red-Black tree and a KD-Tree in order to compare the functionality speed and accuracy of the two data structures in recommendations.</p>

<p align="center"><img src="https://img.shields.io/github/contributors/ebaucom21/Amazon-Entertainment-Catalog-Recommender" alt="shields"><img src="https://img.shields.io/github/commit-activity/w/ebaucom21/Amazon-Entertainment-Catalog-Recommender" alt="shields"></p>

<h2>🍰 Installing SFML to run code:</h2>
**NOTE: This project was written in Clion, these steps will work for Clion garunteed. Other IDE's are untested so use to your discretion.**

1. Go to https://www.sfml-dev.org/download/sfml/2.5.1 and download the SFML package library that corresponds to your system(either GCC 7.3.0 MinGW (SEH) - 64-bit or GCC 7.3.0 MinGW (DW2) - 32-bit). For example, a 64-bit machine running MinGW, you will download the package here:

![image-8c93f29f-e42d-4cf7-9393-a35740412e98](https://github.com/user-attachments/assets/0d6abe19-d047-40e8-baa7-4fe90d3998c1)
2. Once the download is complete, locate it and extract the zip file somewhere on your computer. It shouldn't matter where you have it, but you must keep it there while running this code.

3. In the SFML-2.5.1 folder you extracted, go to the folder named bin. Inside this folder you should see the following:
<img width="477" alt="Screenshot 2025-04-22 165824" src="https://github.com/user-attachments/assets/a108c32b-40a4-4a19-b6b2-628bd578e71f" />

4. Copy all files and paste them into your cmake-build-debug folder in your CLion project. It should look like this in your CLion:
![image-f62e23e5-7c50-4d08-9c48-df7904240918](https://github.com/user-attachments/assets/75d56693-7e8a-4fb5-aa7c-95bcd6a79754)



5. Now we need to get the sfml root prefix. Open the SFML-2.5.1 file you downloaded earlier and copy the path to that folder. This can be done by clicking on the path bar near the top of your File Explorer window:

![image-8637238d-ee18-47c8-a054-3505bfa30c61](https://github.com/user-attachments/assets/f2e8ebb8-e3ea-4536-8993-d7706f784170)

6. Copy that path and paste it into the CMakeLists.txt file, replacing the file path that currently exists. Make sure the end says "/lib/cmake/SFML". Be sure to replace all back-slashes ( \ ) with forward-slashes ( / ).

<img width="804" alt="Screenshot 2025-04-21 235903" src="https://github.com/user-attachments/assets/37c11797-3c2f-4022-b900-fe5b299423f8" />



7. After this step, in some instances your IDE may not automatically reset the cache and reload the project. In order to do so manually, go to: Tools > CMake > Reset Cache and Reload Project

![image-c385e768-6885-4504-8131-cc35694ac54e](https://github.com/user-attachments/assets/ae5e695a-6c64-4ec5-87e9-23cdf21f4067)



<h2>🍰 Contribution Guidelines:</h2>

Please create a new branch for any changes you will be creating. When finished on your local branch you may create a pull request to main and it will be required to be reviewed before it is able to be pushed through to main.
