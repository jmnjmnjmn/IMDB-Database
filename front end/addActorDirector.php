<!DOCTYPE html>
<html>
	<body>
				Add new actor/director: <br/>
		<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="GET">
			Identity:	<input type="radio" name="identity" value="Actor" checked="true">Actor
						<input type="radio" name="identity" value="Director">Director<br/>
			<hr/>
			First Name:	<input type="text" name="first" maxlength="20"><br/>
			Last Name:	<input type="text" name="last" maxlength="20"><br/>
			Sex:		<input type="radio" name="sex" value="Male" checked="true">Male
						<input type="radio" name="sex" value="Female">Female<br/>
						
			Date of Birth:	<input type="text" name="dob"> YYYY-MM-DD <br/>
			Date of Die:	<input type="text" name="dod"> YYYY-MM-DD (leave blank if alive now)<br/>
			<input type="submit" value="Add it!!"/>
		</form>
<?php 

if($first = $_GET["first"]){

	$db_connection=mysql_connect('localhost',"cs143","");
	mysql_select_db("CS143", $db_connection);

	$identity = $_GET["identity"];
	$last = $_GET["last"];
	$sex = $_GET["sex"];
	$dob = $_GET["dob"];
	$dod = $_GET["dod"];

	if (preg_match("/\d\d\d\d-\d\d-\d\d/", $dob ,$matches)){

	$query1 = "SELECT COUNT(*) FROM MaxPersonID;";
	$rs = mysql_query($query1, $db_connection);
	$row = mysql_fetch_row($rs);

	if ($row[0] == 0){
		$query2 = "INSERT INTO MaxPersonID (SELECT MAX(id) FROM Actor);";
		mysql_query($query2, $db_connection);
	}

	$query3 = "UPDATE MaxPersonID SET id = (SELECT MAX(id) FROM Actor)+1;";

	mysql_query($query3, $db_connection);

	$query4 = "SELECT id FROM MaxPersonID;";
	$rs1 = mysql_query($query4, $db_connection);
	$row1 = mysql_fetch_row($rs1);


	if ($dod == NULL) {
		if ($identity == "Actor"){
			$query = "INSERT INTO Actor VALUES($row1[0], \"$last\",\"$first\",\"$sex\",\"$dob\",NULL);";
			mysql_query($query, $db_connection);
		}
		else{
			$query = "INSERT INTO Director VALUES($row1[0], \"$last\",\"$first\",\"$dob\",NULL);";
			mysql_query($query, $db_connection);
		}
	}
	else{
		if ($identity == "Actor"){
			$query = "INSERT INTO Actor VALUES($row1[0], \"$last\",\"$first\",\"$sex\",\"$dob\",\"$dod\");";
			mysql_query($query, $db_connection);
		}
		else{
			$query = "INSERT INTO Director VALUES($row1[0], \"$last\",\"$first\",\"$dob\",\"$dod\");";
			mysql_query($query, $db_connection);
		}
	}

	echo "Add Success!";
	
	}
	else
		echo "Input of Date is invaild!";

	mysql_close($db_connection);

}
?>

		<hr/>
				
	</body>

</html>