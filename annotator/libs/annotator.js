/*
 * JavaScript audio annotator
 * Antonin Delpeuch
 */

var currentClass = -1;
var pop = 0;
var events = Array();
var moveEvt = -1;
var seeking = false;
var canvasX = 0;
var canvasY = 0;
var totalLength = 0;

// Check for the various File API support.
if (!(window.File && window.FileReader && window.FileList && window.Blob))
{
      alert('The File APIs are not fully supported in this browser.');
}

function onClassChange(cl)
{
    var secs = 0;
    if(pop != 0)
    {
        secs = pop.roundTime();
    }

    events[events.length] = { time : secs, cl : cl};
    events.sort(function(a,b){return a.time - b.time});
}


function generateFile()
{
    xml = '<timeline>\n';
    for(var i = 0; i < events.length; i++)
    {
        xml += '    <point time="'+ events[i].time +'" class="'+events[i].cl+'" />\n';
    }
    xml += '</timeline>\n';
    document.outputForm.xmlOutput.value = xml;
}

function updateInterface(cl)
{
    document.getElementById('class-'+cl).style.border
        = '2px solid black';
    if(currentClass != cl && currentClass != -1)
    {
        document.getElementById('class-'+currentClass).style.border
            = '2px solid white';
    }
    
    currentClass = cl;

    onClassChange(cl);
    fillCanvas();
}

function setClass(cl)
{
    if(currentClass != cl)
    {
        updateInterface(cl);
        currentClass = cl;
    }
}

function startAnnotator()
{
    filename = document.getElementById('inputFile').value;
    document.getElementById('audioPlayer').innerHTML =
        '<source src="'+ filename + '" type="audio/ogg" />';
     document.outputForm.xmlOutput.value="";
     updateInterface(0);
     pop = Popcorn('#audioPlayer');
}

function resizeTimeline()
{
    totalLength = pop.duration();
    document.getElementById('timeline').setAttribute("width",
            totalLength.toString());
}

function volumeUp()
{
    pop.volume(pop.volume()+0.1);
}

function volumeDown()
{
    pop.volume(pop.volume()-0.1);
}

function fillCanvas()
{
    var colors = Array();
    colors[0] = "#880000";
    colors[1] = "#008800";
    colors[2] = "#000088";
    colors[3] = "#008888";
    colors[4] = "#888800";
    colors[5] = "#880088";
    colors[6] = "#888888";
    var c = document.getElementById("timeline");
    var ctx = c.getContext("2d");
    var currentColor = colors[0];
    var lastTime = 0;

    ctx.fillStyle = "#FFFFFF";
    ctx.fillRect(0,0,totalLength,50);

    for(var i = 0; i < events.length; i++)
    {
        ctx.fillStyle = currentColor;
        ctx.fillRect(lastTime,0,events[i].time-lastTime,40);
        lastTime = events[i].time;
        currentColor = colors[events[i].cl];
    }

    if(pop != 0 && pop.roundTime() > lastTime)
    {
        ctx.fillStyle = currentColor;
        ctx.fillRect(lastTime,0,pop.roundTime()-lastTime,40);
    }
    if(pop != 0)
    {
        ctx.fillStyle = "#000000";
        ctx.fillRect(pop.roundTime(),0,1,40);
    }
}

function onMouseDown(evt)
{
    var msx = evt.pageX - canvasX;
    if(msx < 0)
    {
        msx = 0;
    }
    var msy = evt.pageY - canvasY; 
    if(msy > 40)
    {
        seeking = true;
        onMouseMove(evt);
    }
    else
    {
        var i = 0;
        while(i < events.length && events[i].time < msx + 2)
        {
            i++;
        }
        i--;
        if(Math.abs(events[i].time - msx) < 2)
        {
            moveEvt = i;
        }
    }
}

function onMouseMove(evt)
{
    var msx = evt.pageX - canvasX;
    var msy = evt.pageY - canvasY; 
    if(moveEvt != -1)
    {
       events[moveEvt].time = msx; 
       fillCanvas();
    }
    if(seeking && pop != 0)
    {
        pop.currentTime(msx);
    }
}

function onMouseUp(evt)
{
    var msx = evt.pageX - canvasX;
    var msy = evt.pageY - canvasY; 
    seeking = false;
    moveEvt = -1;
}

function findPos(obj) {
    var curleft = 0, curtop = 0;
    if (obj.offsetParent)
    {
            do {
                  curleft += obj.offsetLeft;
                  curtop += obj.offsetTop;
                } while (obj = obj.offsetParent);
        return { x: curleft, y: curtop };
    }
    return undefined;
}
var parseXml;

if (typeof window.DOMParser != "undefined") {
        parseXml = function(xmlStr) {
                    return ( new window.DOMParser() ).parseFromString(xmlStr, "text/xml");
     };
} else if (typeof window.ActiveXObject != "undefined" &&
               new window.ActiveXObject("Microsoft.XMLDOM"))
{
       parseXml = function(xmlStr) {
           var xmlDoc = new window.ActiveXObject("Microsoft.XMLDOM");
           xmlDoc.async = "false";
           xmlDoc.loadXML(xmlStr);
           return xmlDoc;
       };
} else {
       throw new Error("No XML parser found");
}

function loadTimeline()
{
    var xmldoc = parseXml(document.outputForm.xmlOutput.value);
    var points = xmldoc.getElementsByTagName("point");

    events = Array();
    for(var i = 0; i < points.length; i++)
    {
        events[events.length] =
          {
            time : points[i].getAttribute("time"), 
            cl : points[i].getAttribute("class")
          };    
    }
    fillCanvas();
}


document.addEventListener('DOMContentLoaded', function ()
        {
     pop = Popcorn('#audioPlayer');
     pop.playbackRate(130);
     updateInterface(0);
   /*  setInterval(function(){ alert(pop.roundTime()) }, 3000); */
     var player = document.getElementById('audioPlayer');
     player.addEventListener(
     'timeupdate', function() { fillCanvas() }, false);
     player.addEventListener(
     'play', function() { resizeTimeline() }, false);
     var timeline = document.getElementById('timeline');
     timeline.addEventListener(
      'mousedown', onMouseDown, false);
     timeline.addEventListener(
      'mousemove', onMouseMove, false);
     timeline.addEventListener(
      'mouseup', onMouseUp, false);

     var pos = findPos(timeline);
     canvasX = pos.x;
     canvasY = pos.y;
        }, false);

