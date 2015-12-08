<html>
<head>
<meta charset="UTF-8">
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
<?php
$mac=$_POST["mac"];
$name=$_POST["name"];
if($name=="Put Your Name" || $name=="")
{
#header("Location:namewrong.html");
echo "<form name='form1' method='post' action='namewrong.php'>\n";
echo "<input type='hidden' name='mac' value='$mac' />\n";
echo "</form>\n";
echo "<script>document.form1.submit();</script>";
exit();
}
$conn=mysql_connect("localhost","test","test");
if(!$conn)
{
die('Could not connect : '.mysql_error());
}
mysql_select_db("bob",$conn);
mysql_query("set names euckr");
$res=mysql_query("select * from user where mac='$mac'",$conn);
if(mysql_fetch_array($res))
{
#header("Location:macdup.html");
echo "<form name='form1' method='post' action='macdup.php'>\n";
echo "<input type='hidden' name='mac' value='$mac' />\n";
echo "</form>\n";
echo "<script>document.form1.submit();</script>";
exit();
}
$query="insert into user (mac,name) values('$mac','$name')";
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
<form name="form1" method='post' action='register.php'>
<input type='hidden' name="mac" value="<?=$mac?>" />
</form>
<script>
setTimeout("document.form1.submit();",1500);
</script>
</div>
</body>
</html>
