<?php
	if (!isset($haveErrors))
	{
		die("Please access this page through one of the table " .
			"management links.<br />");
	};

	if ($haveErrors) {
		die($errorString);
	};
?>

<form method="post" action="../processing/pcSearch.php">
<span class="normal">
<table>
	<tr>
		<td colspan="2">Search for a pc by entering the desired criteria in the
		input boxes provided. Entering nothing in all of the boxes will
		simply cause all records to be displayed.</td>
	</tr>
<?php
	// Print error status if it was passed in the query string.
	if (isset($_REQUEST['error']))
	{
?>
	<tr>
		<td colspan="2" style="background-color: #cc7777">
<?php
		if ($_REQUEST['error'] == 1) {
			echo "Please do not try to access the processing page for this form directly.";
		}
		else {
			echo "Failed to connect to the database!";
		};
?>
		</td>
	</tr>
<?php
	};
?>
	<tr>
		<td>ID:</td>
		<td><input type="text" name="search_id" /></td>
	</tr>
	<tr>
		<td>Description:</td>
		<td><input type="text" name="search_description" /></td>
	</tr>
	<tr>
		<td colspan="2"><input type="submit" value="Search" /></td>
	</tr>
</table>
</span>
</form>

