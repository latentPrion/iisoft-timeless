<?php
	if (!isset($_REQUEST) || !isset($_REQUEST['id']))
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=List&mode=remove&error=1" />

<?php
		die();
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=List&mode=remove&error=2" />

<?php
		die();
	};

	// Send the delete query.
	$queryString = "DELETE * "
		. "FROM pc "
		. "WHERE pc.id = " . $_REQUEST['id'];

	echo $queryString;

	$resultHandle = @odbc_exec($dbHandle, $queryString);
	if (!$resultHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=List&mode=remove&error=3" />

<?php
		die();
	};

	odbc_close($dbHandle);
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=List&mode=remove&error=0" />

