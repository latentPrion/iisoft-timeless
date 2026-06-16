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

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Add&error=1" />

<?php
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Add&error=2" />

<?php
		die();
	};

	// Post vars are available, so we can use them and add the new customer.
	$queryString = "insert into customer (
			username, password, fullName, email, address, contactNo, joinDate, timeRemaining)
		values (
			"
			. "'" . $_POST['customer_username'] . "', "
			. "'" . $_POST['customer_password'] . "', "
			. "'" . $_POST['customer_fullName'] . "', "
			. "'" . $_POST['customer_email'] . "', "
			. "'" . $_POST['customer_address'] . "', "
			. "'" . $_POST['customer_contactNo'] . "', "
			. "'" . $_POST['customer_joinDate'] . "', "
			. "0);";

	$ret = odbc_exec($dbHandle, $queryString);
	if (!$ret)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Add&error=3" />

<?php
	}
	else
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=customer&form=Add&error=0" />

<?php
	};

	odbc_close($dbHandle);
?>

</head>

</html>

