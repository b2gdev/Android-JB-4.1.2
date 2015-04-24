This readme file provides basic instructions on how to build the SLQS
SDK to generate the libraries and executables that are needed by an application
in order to communicate with and use supported modems. A sample application
build and execute are also included. This assumes that the development computer
is already set up.

1) BUILDING THE SDK
   The top level of the provided packages is "pkgs".
   Navigate to this directory and type

   For Intel x86 based systems:

     make -f pkgs.mak complete

   For other platforms, the proper toolchains must be provided. See gen.mak and
   edit the compiler, library and include paths for those platform accordingly.
   To build for these platforms:

     make -f pkgs.mak CPU=arm9 complete

     make -f pkgs.mak CPU=ppc complete

     as the case may be.

1.1) EXECUTING THE SDK ( on an i386 platform )

   Navigate to pkgs/sdk and type

      ./slqssdki386 &

   and the SDK is started up. Note that the program is run in background mode.

   NOTE: - The SDK daemon must be executed with root permissions.

2) BUILDING SAMPLE APPLICATIONS ( illustrated with the sample image management app.
                                  for MC7xx devices running on an i386 platform )

   From the top level directory, navigate to SampleApps/MC77xx_Image_Management/
   and type:

      make

   The executable is created in SampleApps/MC77xx_Image_Management/bin

3) RUNNING SAMPLE APPLICATIONS ( illustrated with the sample image management app.
                                 for MC7xx devices running on an i386 platform )

   To run the sample application, navigate to the bin directory and type:

      ./mc77xximgmgmti386

   NOTES: - Sample Applications should be executed from within their respective
            bin directory as shown above.
          - If the SDK daemon is not running prior to executing the sample app.,
            the sample app. will create the SDK daemon. In that case the sample
            app. must be executed with root permissions.
          - Sample application directories also contain a readme.txt file that
            you are advised to refer to.

