-----
-- Dialog
----

dialogOptions = {};

function showDialogOptions()
	for i, option in ipairs(dialogOptions) do
		message( string.format("%d",i), option );
	end
end
