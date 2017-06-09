function Graph( parentDiv, varargin={} ) {
  // constructor for graph class
  
  // set initial axes ranges to default values
  this.xRng = Graph.DEFAULT_X_RNG
  this.yRng = Graph.DEFAULT_Y_RNG
  
  // set inital line color to default value
  this.color = Graph.DEFAULT_COLOR
  
  // set initial axes labels
  this.xLabelText = Graph.DEFAULT_X_LABEL
  this.yLabelText = Graph.DEFAULT_Y_LABEL
  
  // set inital axis fit events/automation variables
  this.fitYRngEveryMs = Graph.DEFAULT_FIT_Y_EVERY_MS
  this.fitYRngEvent = Graph.DEFAULT_FIT_Y_EVENT
  this.fitYRngResetEvent = Graph.DEFAULT_FIT_Y_RESET_EVENT
  this.copyDataToClipboardEvent = Graph.DEFAULT_COPY_DATA_TO_CLIPOBOARD_EVENT
  
  // set inital fit range to full data set
  this.fitYRngLastNSamples = Graph.MAX_DATA_SIZE
  
  // override any default values if specified in varargin
  for ( key in varargin )
    if ( this.hasOwnProperty( key ) )
      this[key] = varargin[key]
  
  // store the parent container div in case we need to look at it later
  this.parentDiv = parentDiv
  
  // get appropriate time to use for real-time tracking
  this.now = function() { return Date.now() }
  this.tock = this.now()
  
  // flag to stop tick tocking (will trigger next time tick
  // tock function ends)
  this.tickTockStop = false
  
  // flag to know when we are ticking
  this.tickTockRunning = false
  
  // initialize local data container
  this.data = []
  
  // when object is here, we should actually create a graph too,
  // the rest of this constructor is doing exactly this...
  
  // create graph div
  this.div = document.createElement( 'div' )
  this.div.setAttribute( 'id', Graph.DIV_BASE_ID + '_' + Math.random().toString(36).substr(2,9) )
  this.div.classList.add( 'containerItem' )
  this.div.classList.add( 'graphItem' )
  
  // store reference to object in DOM in case we need to get at it
  this.div.obj = this
  
  // and add it to the DOM
  this.parentDiv.appendChild( this.div )
  
  // create graph svg element
  this.graphSvg = d3.select( this.div )
    .append( 'svg' )
  
  // configure graph svg with graph elements
  this.graph = this.graphSvg
    .append( 'g' )
    .attr( 'transform', 'translate(' + Graph.MARGINS.left + ',' + Graph.MARGINS.top + ')' )
  
  // create an initial instance of our axes that we can modify later
  this.xAxis = this.graph.append( 'g' )
    .attr( 'class', 'axis' )
    .call( d3.svg.axis()
      .scale( d3.time.scale()
        .domain( [0,1] )
        .range( [0,1] )
      ).orient( 'bottom' )
    )
  this.yAxis = this.graph.append( 'g' )
    .attr( 'class', 'axis' )
    .call( d3.svg.axis()
      .scale( d3.scale.linear()
        .domain( [0,1] )
        .range( [0,1] ) )
      .orient( 'left' ) 
    )
  
  // add labels to the axes
  this.xLabel = this.graph.append( 'text' )
    .attr( 'dy', '1em' )
    .style( 'text-anchor', 'middle' )
    .text( this.xLabelText )
  this.yLabel = this.graph.append( 'text' )
    .attr( 'transform', 'rotate(-90)' )
    .attr( 'dy', '1em' )
    .style( 'text-anchor', 'middle' )
    .text( this.yLabelText )
  
  // create clip path to hide line elements outside axes
  this.clip = this.graph.append( 'defs' ).append( 'clipPath' )
    .attr( 'id', 'mask' )
    .append( 'rect' )
  
  this.pathContainer = this.graph.append( 'g' )
    .attr( 'clip-path', 'url(#mask)' )
  
  // initially  make a null line to make it our graph empty (but still
  // present)
  this.path = this.pathContainer.append( 'path' )
    .attr( 'class', 'line' )
    .style( 'stoke', this.color )
    .data( [this.data] )
    .attr( 'd', [] )
  
  // now render the graph (it is important to abstract the rest of the
  // functionality so we can utilize this method on window resize
  // events
  this.render()
  
  // also make sure to resize other graphs
  var curGraphCnt = this.getGraphCnt()
  var indexOfGraph = this.getIndexOfGraph()
  var parentDivChildren = this._getGraphChildren()
  for ( var idx=0; idx<curGraphCnt; idx++ )
    if ( idx != (indexOfGraph-1) )
      parentDivChildren[idx].obj.render()
  
  // attach render function to window resize events
  this.lisResizeFuncH = window.addEventListener( 'resize', this.render.bind( this ), false )
  
  // attach function every * ms if variable is set to resize the 
  // y-axis automatically
  if ( this.fitYRngEveryMs )
    this.lisFitYRngH = window.setInterval( this.fitYRng.bind(this), this.fitYRngEveryMs )
  else
    this.lisFitYRngH = null
  
  // attach event listener to interactively resize the y-axis
  if ( this.fitYRngEvent )
    this.div.addEventListener( this.fitYRngEvent, function() { 
      this.fitYRngLastNSamples = 1
      this.fitYRng()
    }.bind(this) )
  
  // attach event listener to reset any changes of resizing the y-axis
  if ( this.fitYRngResetEvent )
    this.div.addEventListener( this.fitYRngResetEvent, function() { 
      this.fitYRngLastNSamples = Graph.MAX_DATA_SIZE
      this.fitYRng()
    }.bind(this) )
  
  if ( this.copyDataToClipboardEvent )
    this.div.addEventListener( this.copyDataToClipboardEvent, function() { 
      stringToClipboard( arrToTsv( formatArr( 
      [].concat.apply( [], this.data.map( function(obj) { 
        return [obj.time,obj.value] } ) ), 2, 0 ) ) )
    }.bind(this) )
    
  return
}

// constants relating to underlying magic
Graph.DIV_BASE_ID= 'graph'
Graph.MAX_DATA_SIZE = 1500

// contants relating to axes stuff
Graph.DEFAULT_X_RNG = [-30*1000,5*1000]
Graph.DEFAULT_Y_RNG = [0,1]
Graph.Y_RNG_BUF = 0.01
Graph.DEFAULT_X_LABEL = ''
Graph.DEFAULT_Y_LABEL = ''

// constants for pop and pep
Graph.DEFAULT_COLOR = 'orange'

// contants relating to visual appeal
Graph.TRANSITION_REFRESH_MS = 100
Graph.TICK_TOCK_REFRESH_MS = 250
Graph.GRAPHS_PER_ROW = 2
Graph.MARGINS = { 
  bottom: 45,
  top: 15,
  left: 60,
  right: 15
}

// constants relating to dynamic events
Graph.DEFAULT_FIT_Y_EVERY_MS = null
Graph.DEFAULT_FIT_Y_EVENT = 'click'
Graph.DEFAULT_FIT_Y_RESET_EVENT = null
Graph.DEFAULT_COPY_DATA_TO_CLIPOBOARD_EVENT = 'dblclick'

Graph.prototype.destroy = function() {
  
  // remove div container from DOM
  this.parentDiv.removeChild( this.div )
  
  // remove event handlers from window
  window.removeEventListener( 'resize', this.lisResizeFuncH )
  if ( this.lisFitYRngH )
    window.clearInterval( this.lisFitYRngH )
  
  // this really doesn't do anything (JS is garbage collected) but it's
  // nice to look at...?
  delete this
  
  return
}

Graph.prototype.render = function() {
  // really, really good function that basically does all initial
  // rendering of the graph elements (this is needed initially, anytime
  // the div is resized, another graph is added, etc.)
  
  console.log( 'rendering graph @' + this.div.id )
  
  // first make sure our graph's svg is the right dimensions
  // to do this we first need information about how many other graphs
  // are present
  var curGraphCnt = this.getGraphCnt()
  var indexOfGraph = this.getIndexOfGraph()
  var rows = Math.ceil( curGraphCnt / Graph.GRAPHS_PER_ROW )
  var graphsInLastRow = curGraphCnt % Graph.GRAPHS_PER_ROW
  var areWeInLastRow = indexOfGraph > ( (rows-1) * Graph.GRAPHS_PER_ROW )
  // then, based on flexbox characteristics, we should set the 
  // width/height of the div
  this.height = this.parentDiv.clientHeight / rows
  if ( areWeInLastRow && (graphsInLastRow!=0) )
    this.width = this.parentDiv.clientWidth / graphsInLastRow
  else
    this.width = this.parentDiv.clientWidth / Graph.GRAPHS_PER_ROW
  this.graphSvg
    .attr( 'width', this.width )
    .attr( 'height', this.height )
  
  // need to resize other graphs
  $( this.parentDiv ).trigger( 'resize' )
  
  // now that our graph div is added, we need to see how big our graph
  // dimensions should actually be
  this.graphWidth = this.width - Graph.MARGINS.left - Graph.MARGINS.right
  this.graphHeight = this.height - Graph.MARGINS.bottom - Graph.MARGINS.top
  //console.log( '  width: ' + this.graphWidth + 'px' )
  //console.log( '  height: ' + this.graphHeight + 'px' )
  
  // get scale for y-axis and update physical axis
  this.xScale = d3.time.scale()
    .domain( [ this.tock + this.xRng[0], this.tock + this.xRng[1] ] )
    .range( [0, this.graphWidth] )
  this.xAxis
    .attr( 'transform', 'translate(0,' + (this.graphHeight) + ')' )
    .transition().call( d3.svg.axis()
      .scale( this.xScale )
      .orient( 'bottom' )
  ).duration( Graph.TRANSITION_REFRESH_MS )
  
  // get scale for y-axis and update physical axis
  this.yScale = d3.scale.linear()
    .domain( [ this.yRng[0], this.yRng[1] ] )
    .range( [ this.graphHeight, 0 ] )
  this.yAxis
    .transition().call( d3.svg.axis()
      .scale( this.yScale )
      .orient( 'left' )
    ).duration( Graph.TRANSITION_REFRESH_MS )
  
  // update position of axes labels
  this.xLabel
    .attr( 'x', this.graphWidth/2 )
    .attr( 'y', this.graphHeight + Graph.MARGINS.bottom )
  this.yLabel
    .attr( 'y', 0 - Graph.MARGINS.left )
    .attr( 'x', 0 - (this.graphHeight/2) )
  
  // update clip path rectangle
  this.clip
    .attr( 'width', this.graphWidth )
    .attr( 'height', this.graphHeight )
    
  // create a function for builing line objects to put on the grpah 
  this.line = d3.svg.line()
    .interpolate( 'linear' ) //( 'basis-open' )
    .x( function (d,i) {
      return this.xScale( d.time )
    }.bind( this ) )
    .y( function(d,i) { 
      return this.yScale( d.value ) 
    }.bind( this ) )
    
  return
}

Graph.prototype._getGraphChildren = function() {
  return children = $( this.parentDiv ).children( 'div' )
}

Graph.prototype.getIndexOfGraph = function( children ) {
  var children = this._getGraphChildren()
  for ( var idx=0; idx<children.length; idx++ ) {
     if ( children[idx].id==this.div.id)
      return idx + 1
  }
  return -1
}

Graph.prototype.getGraphCnt = function() {
  return this._getGraphChildren().length
}

Graph.prototype._setXRng = function( rng, refreshSpeed=500, transition='linear' ) {
  
  // not always needed, xRng is relative to this.tock
  this.xRng = rng
  
  // change domain of axis based on tock (current time)
  this.xScale.domain( [ this.tock + this.xRng[0], this.tock + this.xRng[1] ] )
  
  // update physical axis based on new range
  this.xAxis.transition()
    .duration( refreshSpeed )
    .ease( transition )
    .call( d3.svg.axis()
      .scale( this.xScale )
      .orient( 'bottom' ) 
    )
    
  return
}

Graph.prototype._setYRng = function( rng, refreshSpeed=100, transition='linear' ) {
  
  // don't waste resources if nothing has changed
  if ( rng == this.yRng ) return
  
  // update range object property
  this.yRng = rng
  
  // change domain of axis based on tock (current time)
  this.yScale.domain( [ this.yRng[0], this.yRng[1] ] )
  
  // update physical axis based on new range
  this.yAxis.transition()
    .duration( refreshSpeed )
    .ease( transition )
    .call( d3.svg.axis()
      .scale( this.yScale )
      .orient( 'left' ) 
    )
  
  return
}

Graph.prototype.fitYRng = function( lastNSamples=null ) {
  
  // if local data array is empty, don't mess with axis
  if ( this.data.length == 0 )
    return
  
  // decide subset of data to use to fit y-axis over
  if ( lastNSamples == null )
    lastNSamples = this.fitYRngLastNSamples
  var dataSubset = this.data.slice(
    this.data.length-lastNSamples, 
    this.data.length )
  
  // create new y range based on current data (notice how we are being
  // hip here and using the fancy spread operator, pretty nice right?)
  // UPDATE: just kidding ECMAScript support is spotty...
  this._setYRng( [ 
     d3.min( dataSubset, function (d) { 
      return (1-Graph.Y_RNG_BUF)*d.value 
    } ),
    d3.max( dataSubset, function (d) { 
      return (1+Graph.Y_RNG_BUF)*d.value 
    } )
  ] )
  
  return
}

Graph.prototype.addPoint = function( newTemp, newTime=null ) {
  // helper function to be called to add data and update graph
  
  // no time given, assume now is accurate enough
  if ( !newTime )
    newTime = Date.now()
  
  // add item to data container
  this.data.push( { time: newTime, value: newTemp } )
  
  // container is at its defined limit, or the oldest data point is off the graph, either way remove the oldest item
  if ( ( this.data.length > Graph.MAX_DATA_SIZE ) ||
       ( this.data[0].time < (this.tock+this.xRng[0]) ) )
    this.data.shift()
  
  // if we need to resize the y-axis, do it now
  this.fitYRngLastNSamples++
  if ( ( this.data[this.data.length-1].value < this.yRng[0] ) ||
       ( this.data[this.data.length-1].value > this.yRng[1] ) )
    this.fitYRng()
  
  return
}

Graph.prototype.draw = function() {
  // helper function to update path of data
  
  this.path.attr( 'd', this.line )
    .attr( 'transform', null )
    .transition()
  
  return
}

Graph.prototype.clear = function() {
  // helper function to be called to clear graph data and update visually
  
  console.log( '!!NOT IMPLEMENTED YET!!' )
  
  return
}

Graph.prototype.tickTock = function( 
  single=true, refreshSpeed=50, transition='linear' ) {
  
  //console.log( this.div.id + ' . . . ' + 'tick, tock' )
  
  // set flag to show we are running
  if ( !single )
    this.tickTockRunning = true
  
  // update path before we do anything else
  this.draw()
  
  // get time (not now but of when this transition will finish)
  this.tock = this.now() + refreshSpeed
  
  // we need to know how much the path needs to shift before we change
  // the x scale
  var shiftPathThisMuch = this.xScale( this.tock + this.xRng[0] )
  
  // use helper function to update xScale domain and shift physical axis
  this._setXRng( this.xRng, refreshSpeed=refreshSpeed, transition=transition )
  
  // shift plotted data to the left too
  if ( this.tickTockStop || single ) {
    this.path
      .attr( 'transform', null )
      .transition()
      .duration( refreshSpeed )
      .ease( transition )
      .attr( 'transform', 'translate(' + -shiftPathThisMuch + ')' )
    this.tickTockStop = false
    this.tickTockRunning = false
  } else {
    this.path
      .attr( 'transform', null )
      .transition()
      .duration( refreshSpeed )
      .ease( transition )
      .attr( 'transform', 'translate(' + -shiftPathThisMuch + ',0)' )
      .each( 'end', this.tickTock.bind(this,false,undefined,undefined) )
  }
  
  return
}