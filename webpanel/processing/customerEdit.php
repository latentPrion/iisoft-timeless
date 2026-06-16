<?php
	// Check for invalid page access.
	if (!isset($_POST['edit_id']) || !isset($_POST['edit_valid']) || $_POST['edit_valid'] != 1)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=List&mode=edit" />

<?php
		die();
	};

	// Store new values.
	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Edit&id=<?php echo $_POST['edit_id']; ?>&error=1" />

<?php
		die();
	};

	if ($_POST['edit_newPassword1'] != $_POST['edit_newPassword2'])
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Edit&id=<?php echo $_POST['edit_id']; ?>&error=2" />

<?php
		die();
	};

	// construct update query.
	$queryString = "UPDATE customer "
		. "SET "
		. "fullName = '" . $_POST['edit_fullName'] . "', "
		. "username = '" . $_POST['edit_username'] . "', "
		. "email = '" . $_POST['edit_email'] . "', "
		//. "password = '" . $_POST['edit_newPassword1'] . "', "
		. "address = '" . $_POST['edit_address'] . "', "
		. "timeRemaining = " . $_POST['edit_timeRemaining'] . ", "
		. "contactNo = '" . $_POST['edit_contactNo'] . "' ";

	if ($_POST['edit_newPassword1'] != "") {
		// $queryString .= ", password = '" . $_POST['edit_newPassword1'] . "' ";
	};

	$queryString .= "WHERE id = " . $_POST['edit_id'];

	// Send the query.
	$resultHandle = @odbc_exec($dbHandle, $queryString);
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=List&mode=edit" />

