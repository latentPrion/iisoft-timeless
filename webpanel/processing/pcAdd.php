<html>

<head>

<?php
	/* Check the $_POST[] vars, ensure that the correct form was submitted
	 * etc.
	 **/
	if (!isset($_POST))
	{
		/* If $_POST isn't set, we can just redirect to the correct form
		 * instead of die()ing.
		 **/
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Add&error=1" />

<?php
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Add&error=2" />

<?php
		die();
	};

	// Post vars are available, so we can use them and add the new pc.
	$queryString = "insert into pc (
			description, purchaseDate)
		values (
			"
			. "'" . $_POST['pc_description'] . "', "
			. "'" . $_POST['pc_purchaseDate'] . "')";

	$ret = odbc_exec($dbHandle, $queryString);
	if (!$ret)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Add&error=3" />

<?php
	}
	else
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=pc&form=Add&error=0" />

<?php
	};

	odbc_close($dbHandle);
?>

</head>

</html>

