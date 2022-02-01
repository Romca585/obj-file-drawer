// original code taken from MDN Web Docs
function getdistance() {
   return -4.0;
}

	// position array
	function loadvertexcount() {
           return 3;
        }
// vertex positions
	// v data
function loadvertices() {
    return [
        -1.000000, -1.000000, 1.000000,
        1.000000, -1.000000, 1.000000,
        1.000000, 1.000000, 1.000000,
        ];

}


function loadnormals() {
    return [
        0.000000, 0.000000, 1.000000,
        0.000000, 0.000000, 1.000000,
        0.000000, 0.000000, 1.000000,
        ];
}


function loadtextcoords() {
    return [
        0.000000, 0.000000,
        1.000000, 0.000000,
        1.000000, 1.000000,
        ];
}


function loadvertexindices() {
    return [
        0, 1, 2,
         ];

}


	// texture array size and data
	// width and height of the ppm file

function loadwidth() {
    return 2;
}

function loadheight() {
    return 2;
}

function loadtexture() {
    return( new Uint8Array([
        0, 0, 255, 255, 
        255, 0, 0, 255, 
        0, 255, 0, 255, 
        255, 255, 255, 255
                          ]) );
}
