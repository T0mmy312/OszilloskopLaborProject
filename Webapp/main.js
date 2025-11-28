var canvas = document.getElementById("mainCanvas");
window.addEventListener('resize', resizeCanvas);

const xhr = new XMLHttpRequest();

xhr.open('GET', 'https://jsonplaceholder.typicode.com/posts', true);

xhr.onreadystatechange = function () {
    if (xhr.readyState === 3) {
        const response = xhr.responseText;
        console.log(response);
    }
};

xhr.onload = function () {
    if (xhr.status === 200) {
        console.log("Streaming complete");
    } else {
        console.error("Error:", xhr.statusText);
    }
};

xhr.onerror = function () {
    console.error("Request failed");
};

xhr.send();

var ctx = canvas.getContext("2d")

function resizeCanvas() {
    // CSS size in CSS pixels
    const rect = canvas.getBoundingClientRect();
    const cssWidth = rect.width;
    const cssHeight = rect.height;

    // backing store size in physical pixels for crisp rendering on hi-dpi screens
    const dpr = window.devicePixelRatio || 1;
    const backWidth = Math.round(cssWidth * dpr);
    const backHeight = Math.round(cssHeight * dpr);

    // only update when size changed (avoid unnecessary reflows)
    if (canvas.width !== backWidth || canvas.height !== backHeight) {
        canvas.width = backWidth;
        canvas.height = backHeight;

        // keep CSS size unchanged
        canvas.style.width = cssWidth + 'px';
        canvas.style.height = cssHeight + 'px';

        // map drawing operations to CSS pixels (so 0..cssWidth is logical coords)
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

        draw(); // redraw after resizing
    }
}

function draw() {
    // clear in logical (CSS) pixels
    const rect = canvas.getBoundingClientRect();
    ctx.clearRect(0, 0, rect.width, rect.height);

    // example styling
    ctx.lineWidth = 2;       // logical pixels
    ctx.lineCap = 'round';

    // draw diagonal
    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(rect.width, rect.height); // logical coords (CSS pixels)
    ctx.stroke();
}

draw();