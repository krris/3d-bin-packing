var container, stats;

var camera, scene, renderer;

var cube, plane;

var targetRotation = 0;
var targetRotationOnMouseDown = 0;

var mouseX = 0;
var mouseXOnMouseDown = 0;

var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;

init();
animate();

function loadXMLDoc(dname)
{
if (window.XMLHttpRequest)
  {
  xhttp=new XMLHttpRequest();
  }
else
  {
  xhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
xhttp.open("GET",dname,false);
xhttp.send();
return xhttp.responseXML;
}

function init() {

	container = document.createElement( 'div' );
	document.body.appendChild( container );

	var info = document.createElement( 'div' );
	info.style.position = 'absolute';
	info.style.top = '10px';
	info.style.width = '100%';
	info.style.textAlign = 'center';
	info.innerHTML = 'Drag to spin the cube';
	container.appendChild( info );

//	camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 1, 1000 );
//	camera.position.y = 150;
//	camera.position.z = 500;


	scene = new THREE.Scene();

    var SCREEN_WIDTH = window.innerWidth, SCREEN_HEIGHT = window.innerHeight;
    var VIEW_ANGLE = 45, ASPECT = SCREEN_WIDTH / SCREEN_HEIGHT, NEAR = 0.1, FAR = 20000;
    camera = new THREE.PerspectiveCamera( VIEW_ANGLE, ASPECT, NEAR, FAR);
    scene.add(camera);
    camera.position.set(0,150,400);
    camera.lookAt(scene.position); 


	width_num = 1;
	height_num = 3;
	depth_num = 5;
	x_num = 7;
	y_num = 9;
	z_num = 11;
	// parsing xml
	xmlDoc=loadXMLDoc("cuboids.xml");
	elements=xmlDoc.getElementsByTagName("cuboid");
	
	for (i=0; i<elements.length; i++)
  	{ 
  		width = parseInt(elements[i].childNodes[width_num].textContent);
		height = parseInt(elements[i].childNodes[height_num].textContent);
		depth = parseInt(elements[i].childNodes[depth_num].textContent);
		x = parseInt(elements[i].childNodes[x_num].textContent);
		y = parseInt(elements[i].childNodes[y_num].textContent);
		z = parseInt(elements[i].childNodes[z_num].textContent);
    	var geometry = new THREE.CubeGeometry( width, height, depth);
    	for ( var ii = 0; ii < geometry.faces.length; ii ++ ) {
    		geometry.faces[ ii ].color.setHex( Math.random() * 0xffffff );
    	}

    	var material = new THREE.MeshBasicMaterial( { vertexColors: THREE.FaceColors } );
    
    	cube = new THREE.Mesh( geometry, material );
    	cube.position.x = x;
    	cube.position.y = y;
    	cube.position.z = z;
    	scene.add( cube );
  		
    }


	// Cube

//	var geometry = new THREE.CubeGeometry( 100, 100, 100 );
//
//	for ( var i = 0; i < geometry.faces.length; i ++ ) {
//
//		geometry.faces[ i ].color.setHex( Math.random() * 0xffffff );
//
//	}
//
//	var material = new THREE.MeshBasicMaterial( { vertexColors: THREE.FaceColors } );
//
//	cube = new THREE.Mesh( geometry, material );
//	cube.position.y = 0;
//	scene.add( cube );


	// Plane

	var geometryPlane = new THREE.PlaneGeometry( 200, 200 );
	geometryPlane.applyMatrix( new THREE.Matrix4().makeRotationX( - Math.PI / 2 ) );

	var material = new THREE.MeshBasicMaterial( { color: 0xe0e0e0 } );

	plane = new THREE.Mesh( geometryPlane, material );
	scene.add( plane );

	renderer = new THREE.CanvasRenderer();
	renderer.setSize( window.innerWidth, window.innerHeight );

	container.appendChild( renderer.domElement );

	stats = new Stats();
	stats.domElement.style.position = 'absolute';
	stats.domElement.style.top = '0px';
	container.appendChild( stats.domElement );

//	document.addEventListener( 'mousedown', onDocumentMouseDown, false );
//	document.addEventListener( 'touchstart', onDocumentTouchStart, false );
//	document.addEventListener( 'touchmove', onDocumentTouchMove, false );

	//
    
    controls = new THREE.TrackballControls( camera );

	window.addEventListener( 'resize', onWindowResize, false );

}

function onWindowResize() {

	windowHalfX = window.innerWidth / 2;
	windowHalfY = window.innerHeight / 2;

	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();

	renderer.setSize( window.innerWidth, window.innerHeight );

}

//

function onDocumentMouseDown( event ) {

	event.preventDefault();

	document.addEventListener( 'mousemove', onDocumentMouseMove, false );
	document.addEventListener( 'mouseup', onDocumentMouseUp, false );
	document.addEventListener( 'mouseout', onDocumentMouseOut, false );

	mouseXOnMouseDown = event.clientX - windowHalfX;
	targetRotationOnMouseDown = targetRotation;

}

function onDocumentMouseMove( event ) {

	mouseX = event.clientX - windowHalfX;

	targetRotation = targetRotationOnMouseDown + ( mouseX - mouseXOnMouseDown ) * 0.02;

}

function onDocumentMouseUp( event ) {

	document.removeEventListener( 'mousemove', onDocumentMouseMove, false );
	document.removeEventListener( 'mouseup', onDocumentMouseUp, false );
	document.removeEventListener( 'mouseout', onDocumentMouseOut, false );

}

function onDocumentMouseOut( event ) {

	document.removeEventListener( 'mousemove', onDocumentMouseMove, false );
	document.removeEventListener( 'mouseup', onDocumentMouseUp, false );
	document.removeEventListener( 'mouseout', onDocumentMouseOut, false );

}

function onDocumentTouchStart( event ) {

	if ( event.touches.length === 1 ) {

		event.preventDefault();

		mouseXOnMouseDown = event.touches[ 0 ].pageX - windowHalfX;
		targetRotationOnMouseDown = targetRotation;

	}

}

function onDocumentTouchMove( event ) {

	if ( event.touches.length === 1 ) {

		event.preventDefault();

		mouseX = event.touches[ 0 ].pageX - windowHalfX;
		targetRotation = targetRotationOnMouseDown + ( mouseX - mouseXOnMouseDown ) * 0.05;

	}

}

//

function animate() {

	requestAnimationFrame( animate );

	render();
    controls.update();
	stats.update();

}

function render() {

	plane.rotation.y = cube.rotation.y += ( targetRotation - cube.rotation.y ) * 0.05;
	renderer.render( scene, camera );

}

