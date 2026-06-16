<html>

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

<meta http-equiv="refresh" content="1;../form.php?table=employee&form=Add&error=1" />

<?php
		die();
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=employee&form=Add&error=2" />

<?php
		die();
	};

	// Post vars are available, so we can use them and add the new customer.
	$queryString = "insert into employee (
			fullName, email, address, contactNo, jobDescription, salary)
		values (
			"
			. "'" . $_POST['employee_fullName'] . "', "
			. "'" . $_POST['employee_email'] . "', "
			. "'" . stripslashes(str_replace("'", "''", $_POST['employee_address'])) . "', "
			. "'" . $_POST['employee_contactNo'] . "', "
			. "'" . stripslashes(str_replace("'", "''", $_POST['employee_jobDescription'])) . "', "
			. $_POST['employee_salary']
			. ");";

	echo $queryString;
	$ret = odbc_exec($dbHandle, $queryString);
	if (!$ret)
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=employee&form=Add&error=3" />

<?php
		die();
	}
	else
	{
?>

<meta http-equiv="refresh" content="1;../form.php?table=employee&form=Add&error=0" />

<?php
	};

	odbc_close($dbHandle);
?>

</html>

