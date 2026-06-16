
<?php
	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>
<meta http-equiv="refresh" content="0; hawk.php" />
<?php
		die();
	};

	// Just set the flag and exit.
	$queryString = "UPDATE session SET closed = 1 WHERE id = " . $_REQUEST['id'];
	$resultHandle = odbc_exec($dbHandle, $queryString);
	odbc_close($dbHandle);
?>
<meta http-equiv="refresh" content="0; hawk.php" />

