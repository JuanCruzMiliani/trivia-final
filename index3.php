<??php
	//conectamos Con el servidor
	$conectar=@mysqli_connect('localhost','root','',"test");
	//verificamos la conexion
	if(!$conectar){
		echo"No Se Pudo Conectar Con El Servidor";
	}else{

		$base=mysqli_select_db('prueba');
		if(!$base){
			echo"No Se Encontro La Base De Datos";
		}
	}
	//recuperar las variables
	$nombre=$_POST['nombre'];
	$correo=$_POST['correo'];
	$mensaje=$_POST['mensaje'];
	//hacemos la sentencia de sql
	$sql="INSERT INTO datos VALUES('$nombre',
								   '$correo',
								   '$mensaje')";
	//ejecutamos la sentencia de sql
	$ejecutar=mysql_query($sql);
	//verificamos la ejecucion
	if(!$ejecutar){
		echo"Hubo Algun Error";
	}else{
		echo"Datos Guardados Correctamente<br><a href='index.html'>Volver a r</a>
