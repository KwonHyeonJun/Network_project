<html>
<head>
<title> Mac duplicated! </title>
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
?>
<div id="pp" style="margin-top:50%; text-align:center">
<p>You Already</p>
<p>Registered</p>
<form name="form1" method='post' action='register.php'>
<input type='hidden' name="mac" value="<?=$mac?>" />
</form>
<script>
setTimeout("document.form1.submit();",1500);
</script>
</div>
</body>
</html>

