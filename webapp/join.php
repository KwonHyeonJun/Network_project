<html>
<head>
<title> Thank You For Using GAURDIAN! </title>
<style>
p{
font-size:30px;
text-align:center;
color:white;
font-weight:bold;
margin-bottom:2%;
}
</style>
</head>
<body style="background-color:black">
<?php header('Content-Type:text/html;charset-UTF-8');
$mac=$_POST["mac"];

$conn=mysql_connect("localhost","test","test");
if(!$conn)
{
die('Could not connect : '.mysql_error());
}
mysql_select_db("bob",$conn);
$query="insert into user (mac,name) values('$mac','$_POST[name]')";
echo $_POST["name"];
if(!mysql_query($query,$conn))
{
die('Error : '.mysql_error());
echo "fail";
}
mysql_close($conn);

?>
<div id="pp" style="margin-top:50%; text-align:center">
<p>Registeration</p>
<p>Complete</p>
</div>
</body>
</html>
