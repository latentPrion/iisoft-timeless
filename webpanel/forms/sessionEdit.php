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

Please access this page through the <a href="form.php?table=session&form=List&mode=edit">Edit session</a> form.

<?php
		die();
	};

	$dbHandle = odbc_connect("tmsdb", "", "");
	if (!$dbHandle)
	{
?>

Failed to connect to the database!

<?php
		die();
	};

	$queryString = "SELECT * "
		. "FROM session "
		. "WHERE session.id = " . $_REQUEST['id'];

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

<form action="processing/sessionEdit.php" method="post">
<table>
	<tr>
		<td colspan="2">Editing session #<?php echo $_REQUEST['id']; ?></td>
	</tr>
	<tr>
		<td>Start time:</td>
		<td><input type="text" name="edit_startTime" value="<?php echo odbc_result($resultHandle, 'startTime'); ?>" /></td>
	</tr>
	<tr>
		<td>Time Remaining:</td>
		<td><input type="text" value="<?php echo odbc_result($resultHandle, 'timeRemaining'); ?>" name="edit_timeRemaining" /></td>
	</tr>
	<tr>
		<td colspan="2">
			<input type="submit" value="Edit session" />
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

