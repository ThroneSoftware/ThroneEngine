https://www.boost.org/doc/libs/1_62_0/more/getting_started/windows.html

For windows:
Open cmd at the root of the boost version you're trying to build. In this case it is Vendors/Boost/boost_1_71_0

-Call bootstrap.bat
-Call b2 -j8 toolset=msvc-14.1 address-model=64 architecture=x86 link=static threading=multi runtime-link=shared --build-type=complete stage

Delete the bin.v2 folder and any other temporary artefacts.

Delete the doc if not already done.