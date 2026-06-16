<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};

	if (!isset($_REQUEST['id']))
	{
?>

Please access this page through the <a href="form.php?table=employee&form=List&mode=edit">Edit employee</a> form.

<?php
		die();
	};

	$dbHandle = @odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

Failed to connect to the database!

<?php
		die();
	};

	$queryString = "SELECT * "
		. "FROM employee "
		. "WHERE employee.id = " . $_REQUEST['id'];

	$resultHandle = @odbc_exec($dbHandle, $queryString);
	if (!$resultHandle)
	{
?>

Query failed!

<?php
		die();
	};

	if (odbc_fetch_row($resultHandle))
	{
?>

<form action="processing/employeeEdit.php" method="post">
<table>
	<tr>
		<td colspan="2">Editing employee #<?php echo $_REQUEST['id'] . ": \"" . odbc_result($resultHandle, "fullName") . "\""; ?></td>
	</tr>
	<tr>
		<td>Name:</td>
		<td><input type="text" name="edit_fullName" value="<?php echo odbc_result($resultHandle, 'fullName'); ?>" /></td>
	</tr>
	<tr>
		<td>Email address:</td>
		<td><input type="text" name="edit_email" value="<?php echo odbc_result($resultHandle, 'email'); ?>" /></td>
	</tr>
	<tr>
		<td colspan="2">
			<input type="submit" value="Edit employee" />
			<input type="hidden" name="edit_valid" value="1" />
			<input type="hidden" name="edit_id" value="<?php echo $_REQUEST['id']; ?>" />
		</td>
	</tr>
</table>
</form>

<?php
	};

	odbc_close($dbHandle);
?>

