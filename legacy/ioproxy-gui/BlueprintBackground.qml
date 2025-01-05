import QtQuick 2.15

Rectangle {
	id: root
	color: "#133c8c"

	Canvas {
		id: canvas
		anchors.fill: parent
		onPaint: {
			var ctx = getContext("2d");
			var w = canvas.width;
			var h = canvas.height;

			drawGrid(ctx, w, h, 20, 20, 1);
			drawGrid(ctx, w, h, 100, 100, 3);
		}
	}

	function drawGrid(ctx, w, h, squareW, squareH, lineWidth = 1, strokeColor = "#36589c") {
		ctx.beginPath();
		for (var y = squareH; y < h; y += squareH) {
			ctx.moveTo(0, y);
			ctx.lineTo(w, y);
		}
		for (var x = squareW; x < w; x += squareW) {
			ctx.moveTo(x, 0);
			ctx.lineTo(x, h);
		}
		ctx.strokeStyle = strokeColor;
		ctx.lineWidth = lineWidth;
		ctx.stroke();
	}
}
