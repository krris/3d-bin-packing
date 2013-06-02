3D Bin Packing


Usage: 3dBinPacking [-shelf | -guillotine | -global_guillotine] width depth [-f in_file | -r arg arg] -o out_file [-t]
Options:" << endl;
    -shelf \t\t: Shelf algorithm + guillotine algorithm (with initial cuboids sorting)
    -guillotine \t: Guillotine algorithm (with initial cuboids sorting)
    -global_guillotine: Global guillotine algorithm (without initial sorting)
    -width, depth\t: Size of a cuboid base.
    -f : Input xml file with information about cuboids to put.
    -r : Generate n random cuboids, with parameter:
        1 - every edge has different length (default)
        2 - two edges have the same length
        3 - three edges have the same length
    -o : Output file which will store information about placed cuboids.
    -t : Time measurement enabled.

Generating only cuboids is available.
Usage: 3dBinPacking -r number_of_cuboids parameter -o out_file

Example:
$ ./3dBinPacking -shelf 300 250 -f input_file.xml -o output_file.xml -t
$ ./3dBinPacking -shelf 300 250 -r 1000 2 -o output_file.xml -t
$ ./3dBinPacking -r 1000 2 -o output_file.xml


Required libraries:
1) boost

Compilation:
$ scons

Visualization:
1) To visualize output xml file should be saved in directory: visual/app as "cuboids_to_render.xml"
2) $cd visual/app
3) $ python -m SimpleHTTPServer 8005
4) Open your web browser: http://localhost:8005/cuboids.html
