<?php
	// Check for invalid page access.
	if (!isset($_POST['edit_id']) || !isset($_POST['edit_valid']) || $_POST['edit_valid'] != 1)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=List&mode=edit" />

<?php
		die();
	};

	// Store new values.
	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=Edit&id=<?php echo $_POST['edit_id']; ?>&error=1" />

<?php
		die();
	};

	// construct update query.
	$queryString = "UPDATE session "
		. "SET "
		. "timeRemaining = " . $_POST['edit_timeRemaining'] . ", "
		. "startTime = '" . $_POST['edit_startTime'] . "' ";

	$queryString .= "WHERE id = " . $_POST['edit_id'];

	// Send the query.
	$resultHandle = odbc_exec($dbHandle, $queryString);
?>

<meta http-equiv="refresh" content="1;../form.php?table=session&form=List&mode=edit" />

