let t1, t2;
let loopId;
let inputImgData;
const webcamWidth = 640;
const webcamHeight = 480;
const outputCanvas = document.getElementById("output-canvas");
const outputOverlayCanvas = document.getElementById("output-overlay-canvas");
const inputCanvas = document.getElementById("input-canvas");
const video = document.getElementById("video");
outputCanvas.width = webcamWidth;
outputCanvas.height = webcamHeight;
outputOverlayCanvas.width = webcamWidth;
outputOverlayCanvas.height = webcamHeight;
inputCanvas.width = webcamWidth;
inputCanvas.height = webcamHeight;
const outputCtx = outputCanvas.getContext("2d");
const outputOverlayCtx = outputOverlayCanvas.getContext("2d");
const inputCtx = inputCanvas.getContext("2d");
let toGrayscale = document.getElementById("toGrayscale");
let threshold = document.getElementById("threshold");
let corners = document.getElementById("findCorners");
let segments = document.getElementById("findSegments");
let segmentVisualizer = document.getElementById("visualizeSegments");
let centroids = document.getElementById("findCentroids");
let perimeter = document.getElementById("findPerimeter");
let bounding = document.getElementById("findBoundingBox");
function start() {
	loopId = window.requestAnimationFrame(Module._update);
}
function stop() {
	window.cancelAnimationFrame(loopId);
}