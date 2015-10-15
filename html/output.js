$(function()
{
  var table = $('#table');
  var grid = $('#gantt_grid');

  $("#stat_proc").html(g_process.stats.total);
  $("#stat_turn").html(g_process.stats.turnaround);
  $("#stat_wait").html(g_process.stats.wait);
  
  var process_n = g_process.stats.total;
  var hue_table = new Array();

  var process = g_process.process;
  for(var pid in process)
  {
    var proc = process[pid];
    var arrival = proc[0];
    var term = proc[1];
    var burst = proc[2];
    var pri = proc[3];
    var turn = proc[4];
    var wait = proc[5];
    var hue = Object.keys(hue_table).length/process_n*360;
    hue_table[pid] = hue;
    
    table.append($('<tr id="proc_'+pid+'"><td>&nbsp;<span style="background-color:hsl('+hue+',100%,85%)">&nbsp;&nbsp;</span> '+pid+'</td><td>'+arrival+'</td><td>'+term+'</td><td>'+burst+'</td><td>'+pri+'</td><td>'+turn+'</td><td>'+wait+'</td></tr>').mouseenter({pid:pid},
    function(data)
    {
      $('.ev_'+data.data.pid).toggleClass('ev-hover',true);
    }).mouseleave({pid:pid},
    function(data)
    {
      $('.ev_'+data.data.pid).toggleClass('ev-hover',false);
    }));
  }
  
  var end = 0;
  for(var i=0; i<g_event.length; ++i)
  {
    var event = g_event[i];
    var start = event[0];
    var stop = event[1];
    var pid = event[2];
    if(stop > end)
      end = stop;
    grid.append($('<div class="ev_'+pid+'">'+pid+'</div>').css({"top":40,"height":45,"left":start*20,"width":(stop-start)*20-1,'background-color':'hsl('+hue_table[pid]+',100%,85%)'}).mouseenter({ind:i},
    function(data)
    {
      var event = g_event[data.data.ind];
      var start = event[0];
      var stop = event[1];
      var pid = event[2];
      var pri = event[3];

      $('#proc_'+pid).toggleClass('proc-hover',true);
      
      $('#info-pid').html(pid);
      $('#info-pri').html(pri);
      $('#info-start').html(start);
      $('#info-stop').html(stop);
      $('#info').show();
    }).mouseleave({ind:i},
    function(data)
    {
      var event = g_event[data.data.ind];
      var pid = event[2];

      $('#proc_'+pid).toggleClass('proc-hover',false);
      $('#info').hide();
    }).mousemove(
    function(event)
    {
      $('#info').css({"top":event.pageY,"left":event.pageX});
    }));
  }
  grid.width(end*20+1);
  for(tick=0; tick<end; tick+=10)
  {
    grid.append($('<span></span>').html(tick).css({"top":2,"left":tick*20+4}));
  }
});
