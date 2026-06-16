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

<form method="post" action="../processing/customerReport.php">
<span class="normal">
<table>
	<tr>
	<td colspan="3">Please select the options and date ranges for the desired report.</td>
	</tr>
	<tr>
		<td>Join date:</td>
		<td>From:</td>
		<td>To:</td>
	<tr>
		<td></td>
		<td>
			<select name="startYear">
			<option value="0" default>All years</option>
			<option value="1991">1991</option>
			<option value="1992">1992</option>
			<option value="1993">1993</option>
			<option value="1994">1994</option>
			<option value="1995">1995</option>
			<option value="1996">1996</option>
			</select>
		</td>
		<td>
			<select name="endYear">
			<option value="0" default>All years</option>
			<option value="1991">1991</option>
			<option value="1992">1992</option>
			<option value="1993">1993</option>
			<option value="1994">1994</option>
			<option value="1995">1995</option>
			<option value="1996">1996</option>
			</select>
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<select name="startMonth">
			<option value="0" default>All months</option>
			<option value="1">Jan</option>
			<option value="2">Feb</option>
			<option value="3">Mar</option>
			<option value="4">Apr</option>
			<option value="5">May</option>
			<option value="6">Jun</option>
			</select>
		</td>
		<td>
			<select name="endMonth">
			<option value="0" default>All months</option>
			<option value="1">Jan</option>
			<option value="2">Feb</option>
			<option value="3">Mar</option>
			<option value="4">Apr</option>
			<option value="5">May</option>
			<option value="6">Jun</option>
			</select>
		</td>
	</tr>
	<tr>
		<td></td>
		<td><input type="text" value="<All days>" name="startDate"
			onfocus="javascript: if (this.value=='<All days>') { this.value=''; };"
			onblur="javascript: if (this.value=='') { this.value='<All days>'; };" /></td>
		<td><input type="text" value="<All days>" name="endDate"
			onfocus="javascript: if (this.value=='<All days>') { this.value=''; };"
			onblur="javascript: if (this.value=='') { this.value='<All days>'; };"/></td>
	</tr>
	<tr>
		<td>Number of items per page:</td>
		<td colspan="2"><input type="text" value="50" name="nPerPage" /></td>
	</tr>
	<tr>
		<td><label for="checkbox_rawReport">Generate "<i>Print version</i>" report?</label></td>
		<td style="vertical-align: top"><input type="checkbox" name="rawReport" id="checkbox_rawReport"/>(Select to enable)</td>
	</tr>
	<tr>
		<td>Account types:</td>
		<td>
			<input type="radio" value="0" name="accType" id="radio_accType_normal" />
			<label for="radio_accType_normal">Normal.</label>
		</td>
	</tr>
	<tr>
		<td></td>
		<td colspan="2">
			<input type="radio" value="1" name="accType" id="radio_accType_subscribed" />
			<label for="radio_accType_subscribed">Subscribed customers.</label>
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<input type="radio" value="2" name="accType" id="radio_accType_both" checked />
			<label for="radio_accType_both">Both.</label>
		</td>
	</tr>
	<tr style="vertical-align: top">
		<td><label for="checkbox_lostCust">Show only customers within the range who have not logged in since:</label></td>
		<td><input type="checkbox" name="lostCust" id="checkbox_lostCust" />(Select to enable)</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<select name="lostCustYear">
			<option value="0" default>All years</option>
			<option value="1991">1991</option>
			<option value="1992">1992</option>
			<option value="1993">1993</option>
			<option value="1994">1994</option>
			<option value="1995">1995</option>
			<option value="1996">1996</option>
			</select>
			<br />
			<select name="lostCustMonth">
			<option value="0" default>All months</option>
			<option value="1">Jan</option>
			<option value="2">Feb</option>
			<option value="3">Mar</option>
			<option value="4">Apr</option>
			<option value="5">May</option>
			<option value="6">Jun</option>
			</select>
			<br />
			<input type="text" value="<Day>" name="lostCustDay" 
				onFocus="javascript: if (this.value=='<Day>') { this.value=''; };"
				onBlur="javascript: if (this.value=='') { this.value='<Day>'; };" />
		</td>
	</tr>
	<tr>
		<td><label for="checkbox_columnProject">Show only the following columns:</label></td>
		<td><input type="checkbox" name="columnProject" id="checkbox_columnProject" />(Select to enable)</td>
	</tr>
	<tr>
		<td></td>
		<td><input type="checkbox" id="checkbox_columnProject_id" name="columnProject_id" /><label for="checkbox_columnProject_id">Customer ID</label></td>
	</tr>
	<tr>
		<td></td>
		<td><input type="checkbox" id="checkbox_columnProject_name" name="columnProject_name" /><label for="checkbox_columnProject_name">Customer name</label></td>
	</tr>
	<tr>
		<td></td>
		<td><input type="checkbox" id="checkbox_columnProject_email" name="columnProject_email" /><label for="checkbox_columnProject_email">Customer email</label></td>
	</tr>
	<tr>
		<td colspan="3"><input type="submit" value="Generate report" />
	</tr>
</table>
</span>
</form>

