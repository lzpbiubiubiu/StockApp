import QtQuick 2.12
import "./"

// 复选按钮控件
UIImageButton {
    id: checkbox
    property bool isChecked: false
    property string checkedImg: ""
    property string unCheckedImg: ""

    signal signalChecked(bool isChecked)

    onSignalClicked :
	{
		isChecked = !isChecked
        if(isChecked) {
            imageSource = checkedImg
            imageHoverSource = checkedImg
            imagePressSource = checkedImg
        }
        else {
            imageSource = unCheckedImg
            imageHoverSource = unCheckedImg
            imagePressSource = unCheckedImg
        }
        signalChecked(isChecked)
	}

    imageSource: isChecked ? checkedImg : unCheckedImg
    imageHoverSource: isChecked ? checkedImg : unCheckedImg
    imagePressSource: isChecked ? checkedImg : unCheckedImg
}
