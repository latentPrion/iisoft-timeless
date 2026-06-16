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

<form method="post" action="../processing/employeeAdd.php">
<table>
<?php
	if (isset($_REQUEST['error']))
	{
?>
	<tr>
		<td colspan="2" style="background: <?php if ($_REQUEST['error'] != 0) { echo '#cc7777'; } else { echo '#77cc77'; }; ?>;">
<?php
		switch ($_REQUEST['error'])
		{
		default: echo "Insertion of new employee was successful!";
			break;

		case 1: echo "Please do not attempt to access the back end page directly. Use this form.";
			break;

		case 2: echo "Connection to database was unsuccessful.";
			break;

		case 3: echo "Insert query for the new record failed.";
			break;
		};
?>
		</td>
	</tr>
<?php
	};
?>
	<tr>
	<td>Employee name:</td>
	<td><input type="text" maxlength="64" name="employee_fullName" size="50" /></td>
	</tr>
	<tr>
	<td>Employee email:</td>
	<td><input type="text" maxlength="64" name="employee_email" size="50" /></td>
	</tr>
	<tr>
	<td>Employee address:</td>
	<td><input type="text" maxlength="64" name="employee_address" size="50" /></td>
	</tr>
	<tr>
	<td>Contact number:</td>
	<td><input type="text" maxlength="64" name="employee_contactNo" size="50" /></td>
	</tr>
	<tr>
	<td>Date hired:</td>
	<td><input type="text" maxlength="64" name="employee_dateHired" size="50"  value=""/></td>
	</tr>
	<tr>
	<td>Job description:</td>
	<td><input type="text" maxlength="64" name="employee_jobDescription" size="50"  value=""/></td>
	</tr>
	<tr>
	<td>Monthly salary:</td>
	<td><input type="text" maxlength="64" name="employee_salary" size="50"  value=""/></td>
	</tr>
	<tr><td></td></tr>
	<tr>
	<td colspan="2"><input type="submit" value="Add new employee" /></td>
	</tr>
</table>
</form>

