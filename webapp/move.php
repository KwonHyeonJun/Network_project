<?php
$mac=$_POST["mac"];
echo "<html>\n";
echo "<body onload='document.form1.submit();'>\n";
echo "<form name='form1' method='post' action='/finish.php'>\n";
echo "<input type='hidden' name='mac' value='$mac'>\n";
echo "</form>\n";
echo "</body>\n";
echo "</html>";
?>
