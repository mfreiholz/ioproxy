import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import "App.js" as AppJS

ApplicationWindow {
	property var currentBlueprintView

	id: window
	width: 1024
	height: 768
	visible: true

	menuBar: MenuBar {
		Menu {
			title: qsTr("&File")

			Action {
				text: qsTr("&New...")
				onTriggered: function() {
					if (window.currentBlueprintView) {
						window.currentBlueprintView.destroy()
						window.currentBlueprintView = null
					}
					d.newBlueprint()
				}
			}
			Action {
				text: qsTr("&Open...")
			}
			Action {
				text: qsTr("&Close")
				onTriggered: function() {
					if (window.currentBlueprintView) {
						window.currentBlueprintView.destroy()
						window.currentBlueprintView = null
					}
				}
			}
			Action {
				text: qsTr("&Quit")
				onTriggered: function() {
					Qt.quit()
				}
			}
		}
	}

	Component {
		id: blueprintViewComp

		BlueprintView {
			anchors.fill: parent
		}
	}

	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	QtObject {
		id: d

		function newBlueprint() {
			var bp = AppObject.newBlueprint()
			window.currentBlueprintView = blueprintViewComp.createObject(window.contentItem, { bp: bp })
		}
	}

	Component.onCompleted: function() {
		console.log(`Version: ${AppObject.version}`);

		// Auto-Load.
		if (true) {
			d.newBlueprint()
		}
	}
}
