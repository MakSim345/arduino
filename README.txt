23-APR-2013

=========================================================================================
#Arduino
=========================================================================================

My DIY arduino projects
Repository for my arduino sketches.

=========================================================================================
1. Installing libraries

For information on installing libraries, see: "http://www.arduino.cc/en/Guide/Libraries"

Easyest way:
- copy folder with a library to "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries"
- open "Sketch->Include Library" and select new library
- it may add the header file to source code.
- restart the Arduino IDE!

=========================================================================================
2. Manual installation of a libraries

When you want to add a library manually, you need to download it as a ZIP file, expand it and put in the proper directory. The ZIP file contains all you need, including usage examples if the author has provided them. The library manager is designed to install this ZIP file automatically as explained in the former chapter, but there are cases where you may want to perform the installation process manually and put the library in the libraries folder of your sketchbook by yourself.

You can find or change the location of your sketchbook folder at "File > Preferences > Sketchbook location."

=========================================================================================
To add a library to the Arduino IDE, follow these steps:

1. Download the library from its source, which is typically the Arduino Library Manager or a third-party website. The library should be in a ".zip" format.

2. Open the Arduino IDE.

3. Click on the "Sketch" menu at the top of the IDE, then navigate to "Include Library" > "Add .ZIP Library".

4. Select the ".zip" file of the library you downloaded and click "Open".

5. The library will be added to the IDE and you can now use it in your sketches.

6. To verify that the library was added successfully, you can check the "Sketch" menu again and look for the library under "Include Library". If it appears in the list, it was successfully added.

That's it! You can now use the library in your Arduino sketches by including its header files and calling its functions. If you encounter any issues with the library, make sure to check its documentation or support resources for troubleshooting tips.

