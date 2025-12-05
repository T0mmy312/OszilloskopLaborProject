/*
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
*/

var device_select = document.getElementById('device-select');
var dark_mode_button = document.getElementById('dark-mode-button');
var ctx = document.getElementById('chart');

var dark_mode = false;

var dark_color = '#ffffffff'
var light_color = '#2d2d2dff'
var grid_dark_color = '#646464ff'
var grid_light_color = '#a7a7a7ff'

var cfg = {
    type: 'line',
    data: {
        datasets: [{
            data: [
                {x: 0, y: 1},
                {x: 1, y: 2},
                {x: 2, y: 3},
                {x: 3, y: 4}
            ]
        }]
    },
    options: {
        scales: {
            x: {
                type: 'linear',
                color: light_color,
                title: {
                    display: true,
                    text: "t [s]",
                    color: light_color
                },
                ticks: {
                    display: true,
                    color: light_color
                },
                grid: {
                    display: true,
                    color: grid_light_color
                }
            },
            y: {
                type: 'linear',
                color: light_color,
                title: {
                    display: true,
                    text: "U [V]",
                    color: light_color
                },
                ticks: {
                    display: true,
                    color: light_color
                },
                grid: {
                    display: true,
                    color: grid_light_color
                }
            }
        },
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
            title: {
                align: 'right',
                display: true,
                text: 'Oscilloscope Channels',
                color: light_color,
            },
            legend: {
                display: true,
                labels: {
                    color: light_color
                }
            }
        }
    },
};

const chart = new Chart(ctx, cfg);

function buttonClick() {
    console.log("clicked button!");
}

function selectDevice() {
    if (device_select.style.display != 'none')
        device_select.style.display = 'none';
    else
        device_select.style.display = 'block';
}


function toggleDarkMode() {
    dark_mode = !dark_mode;
    let element = document.body;
    element.classList.toggle("dark-mode");
    if (dark_mode) {
        dark_mode_button.innerHTML = "Light Mode";
        cfg.options.plugins.title.color = dark_color;
        cfg.options.plugins.legend.labels.color = dark_color;
        cfg.options.scales.x.title.color = dark_color;
        cfg.options.scales.y.title.color = dark_color;
        cfg.options.scales.x.ticks.color = dark_color;
        cfg.options.scales.y.ticks.color = dark_color;

        cfg.options.scales.x.grid.color = grid_dark_color;
        cfg.options.scales.y.grid.color = grid_dark_color;
    }
    else {
        dark_mode_button.innerHTML = "Dark Mode";
        cfg.options.plugins.title.color = light_color;
        cfg.options.plugins.legend.labels.color = light_color;
        cfg.options.scales.x.title.color = light_color;
        cfg.options.scales.y.title.color = light_color;
        cfg.options.scales.x.ticks.color = light_color;
        cfg.options.scales.y.ticks.color = light_color;

        cfg.options.scales.x.grid.color = grid_light_color;
        cfg.options.scales.y.grid.color = grid_light_color;
    }
    chart.update();
}