<!DOCTYPE html>
<html>
<body>
Type an SQL query in the following box:<br>
<form method="GET" action="<?php echo $_SERVER['PHP_SELF'];?>">
<TEXTAREA NAME="area" ROWS=15 COLS=80>
</TEXTAREA>
<INPUT TYPE="submit" VALUE="done">
</form>

<?php 

if($area = $_GET['area']){

$db_connection=mysql_connect('localhost',"cs143","");
mysql_select_db("CS143", $db_connection);
$rs = mysql_query($area, $db_connection);
echo "<h2>Results from MySQL:</h1>";

echo "<table border=\"1\">";
echo "<tr  align=\"center\">";

while ($field = mysql_fetch_field($rs)){ 
	echo "<td><b>".$field->name."</td>\n"; 
} 
echo "</tr>";

while($row = mysql_fetch_row($rs)) {
    
	echo "<tr  align=\"center\">";
	for($i=0;$i<count($row);$i++){
		if ($row[$i]==null)
			echo "<td>N/A</td>";
		else
			echo "<td>".$row[$i]."</td>";
	}
    echo "</tr>\n";
}

mysql_close($db_connection);
}
?>

</body>
</html>