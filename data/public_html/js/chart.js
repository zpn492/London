var ctx = document.getElementById("myChart").getContext('2d');var myChart = new Chart(ctx, {type: 'line', data: {labels: ["T0.01", "T0.02", "T0.03", "T0.04", "T0.05", "T0.06", "T0.07", "T0.08", "T0.09", "T0.1"],datasets: [{label: 'Some label', pointColor: 'rgba(220,220,220,1', pointStrokeColor: '#fff',  data: [245863, 201891, 166103, 136916, 113067, 93541.4, 77525.7, 64364.5, 53529.3, 44593.1], borderWidth: 1}]}, options: { scales: { yAxes: [{ ticks: { beginAtZero:true } }] } } });