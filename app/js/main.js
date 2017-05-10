var CMDS = {
  // 0x1* debugging commands
  'CMD_LED1_TOG': {
    address:  0x10,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LED1_SET': {
    address:  0x11,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_LED1_GET': {
    address:  0x12,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LED2_TOG': {
    address:  0x13,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LED2_SET': {
    address:  0x14,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_LED2_GET': {
    address:  0x15,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_BTN1_GET': {
    address:  0x16,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_BTN2_GET': {
    address:  0x17,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_DBG_VAL1_GET': {
    address:  0x18,
    rxBytes:  4,
    txBytes:  0,
    },
  'CMD_DBG_VAL2_GET': {
    address:  0x19,
    rxBytes:  4,
    txBytes:  0,
    },
  // 0x2* 
  // 0x3* Ethernet commands 
  // 0x4* delay commands
  'CMD_DELAY_US': {
    address:  0x40,
    rxBytes:  0, 
    txBytes:  2,
    },
  'CMD_DELAY_MS': {
    address:  0x41,
    rxBytes:  0,
    txBytes:  2,
    },
  'CMD_DELAY_S': {
    address:  0x42,
    rxBytes:  0,
    txBytes:  2,
    },
  // 0x5* gimbal commands
  'CMD_GIMBAL_PAN_TOG': {
    address:  0x50,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_GIMBAL_PAN_SET': {
    address:  0x51,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_GIMBAL_PAN_GET': {
    address:  0x52,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_GIMBAL_TILT_TOG': {
    address:  0x53,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_GIMBAL_TILT_SET': {
    address:  0x54,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_GIMBAL_TILT_GET': {
    address:  0x55,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_GIMBAL_PAN_ANG_SET': {
    address:  0x56,
    rxBytes:  0,
    txBytes:  2,
    },  // (2 bytes) / 360 = resolution of 0.00549deg
  'CMD_GIMBAL_PAN_ANG_GET': {
    address:  0x57,
    rxBytes:  2,
    txBytes:  0,
    },  // (2 bytes) / 360 = resolution of 0.00549deg
  'CMD_GIMBAL_TILT_ANG_SET': {
    address:  0x58,
    rxBytes:  0,
    txBytes:  2,
    },  // (2 bytes) / 360 = resolution of 0.00549deg
  'CMD_GIMBAL_TILT_ANG_GET': {
    address:  0x59,
    rxBytes:  2,
    txBytes:  0,
    },  // (2 bytes) / 360 = resolution of 0.00549deg
  // 0x6 quadrant photodiode activities
  'CMD_QP_CH1_VSENSE_GET': {
    address:  0x60,
    rxBytes:  2,
    txBytes:  0
  },
  'CMD_QP_CH2_VSENSE_GET': {
    address:  0x61,
    rxBytes:  2,
    txBytes:  0
  },
  'CMD_QP_CH3_VSENSE_GET': {
    address:  0x62,
    rxBytes:  2,
    txBytes:  0
  },
  'CMD_QP_CH4_VSENSE_GET': {
    address:  0x63,
    rxBytes:  2,
    txBytes:  0
  },
  'CMD_QP_ALL_BULK_RUN': {
    address:  0x64,
    rxBytes:  0,
    txBytes:  4
  },
  'CMD_QP_ALL_BULK_GET': {
    address:  0x65,
    rxBytes:  62,
    txBytes:  0
  },
  // 0x7* laser activities
  'CMD_LSR_LOAD_SWTICH_TOG': {
    address:  0x70,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LSR_LOAD_SWTICH_SET': {
    address:  0x71,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_LSR_LOAD_SWTICH_GET': {
    address:  0x72,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LSR_VREF_VSENSE_GET': {
    address:  0x73,
    rxBytes:  2,
    txBytes:  0,
    },
  'CMD_LSR_ISENSE_GET': {
    address:  0x74,
    rxBytes:  2,
    txBytes:  0,
    },
  'CMD_LSR_TOG': {
    address:  0x75,
    rxBytes:  1,
    txBytes:  0,
    },
  'CMD_LSR_SET': {
    address:  0x76,
    rxBytes:  0,
    txBytes:  1,
    },
  'CMD_LSR_GET': {
    address:  0x77,
    rxBytes:  1,
    txBytes:  0,
    },
  // 0x8* modulation activities
  'CMD_MOD_TOG': {
    address:  0x80,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_SET': {
    address:  0x81,
    rxBytes:  0,
    txBytes:  1,
  },
  'CMD_MOD_GET': {
    address:  0x82,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_FREQ_HZ_SET': {
    address:  0x83,
    rxBytes:  0,
    txBytes:  4,
  },
  'CMD_MOD_FREQ_HZ_GET': {
    address:  0x84,
    rxBytes:  4,
    txBytes:  0,
  },
  'CMD_MOD_HICCUP_GET': {
    address:  0x85,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_DATA_BULK_RUN': {
    address:  0x86,
    rxBytes:  0,
    txBytes:  0,
  },
  'CMD_MOD_DATA_BULK_GET': {
    address:  0x87,
    rxBytes:  62,
    txBytes:  0,
  },
  'CMD_MOD_SIG_LOCK_GET': {
    address:  0x88,
    rxBytes:  1,
    txBytes:  0,
  },
  // 0x9* 
  // 0xA*
  // 0xB*
  // 0xC*
  // 0xD*
  // 0xE*
}

var CONTROLS = [
  [ 'Main', 'Go', null ],
  [ 'Main', 'Stop', null ],
  [ 'Sub-Main', 'Start Poller', startPoller ],
  [ 'Sub-Main', 'Stop Poller', stopPoller ],
  [ 'Sub-Main', 'Start Graph Scrolling', startTickTock ],
  [ 'Sub-Main', 'Stop Graph Scrolling', stopTickTock ],
  [ 'Quadrant Photodiode', 'Channel 1', function() { sendHandler( [ CMDS['CMD_QP_CH1_VSENSE_GET'].address ] ) } ],
  [ 'Quadrant Photodiode', 'Channel 2', function() { sendHandler( [ CMDS['CMD_QP_CH2_VSENSE_GET'].address ] ) } ],
  [ 'Quadrant Photodiode', 'Channel 3', function() { sendHandler( [ CMDS['CMD_QP_CH3_VSENSE_GET'].address ] ) } ],
  [ 'Quadrant Photodiode', 'Channel 4', function() { sendHandler( [ CMDS['CMD_QP_CH4_VSENSE_GET'].address ] ) } ],
  [ 'Quadrant Photodiode', 'Bulk Read', function() { sendHandler( [ CMDS['CMD_QP_ALL_BULK_RUN'].address, 0, 0, 0, 0 ] ) } ],
  [ 'Laser', 'Load Switch', function() { sendHandler( [ CMDS['CMD_LSR_LOAD_SWTICH_TOG'].address ] ) } ],
  [ 'Laser', 'Toggle Laser', function() { sendHandler( [ CMDS['CMD_LSR_TOG'].address ] ) } ],
  [ 'Laser', 'Current Sense', function() { sendHandler( [ CMDS['CMD_LSR_ISENSE_GET'].address ] ) } ],
  [ 'Laser', 'Vref Voltage Sense', function() { sendHandler( [ CMDS['CMD_LSR_VREF_VSENSE_GET'].address ] ) } ],
  [ 'Modulation', 'Modulate', function() { sendHandler( [ CMDS['CMD_MOD_TOG'].address ] ) } ],
  [ 'Modulation', 'Set 10Hz', function() { sendHandler( MOD_CMD_PKT(10) ) } ],
  [ 'Modulation', 'Set 100kHz', function() { sendHandler( MOD_CMD_PKT(100e3) ) } ],
  [ 'Modulation', 'Set 500kHz', function() { sendHandler( MOD_CMD_PKT(500e3) ) } ],
  [ 'Modulation', '-1kHz', function() { sendHandler( MOD_CMD_PKT(modFreqHz-1000) ) } ],
  [ 'Modulation', '-100Hz', function() { sendHandler( MOD_CMD_PKT(modFreqHz-100) ) } ],
  [ 'Modulation', '+100Hz', function() { sendHandler( MOD_CMD_PKT(modFreqHz+100) ) } ],
  [ 'Modulation', '+1kHz', function() { sendHandler( MOD_CMD_PKT(modFreqHz+1000) ) } ],
  [ 'Modulation', '-10Hz', function() { sendHandler( MOD_CMD_PKT(modFreqHz-10) ) } ],
  [ 'Modulation', 'Current Rate', function() { sendHandler( [ CMDS['CMD_MOD_FREQ_HZ_GET'].address ] ) } ],
  [ 'Modulation', 'Lock State', function() { sendHandler( [ CMDS['CMD_MOD_SIG_LOCK_GET'].address ] ) } ],
  [ 'Modulation', '+10Hz', function() { sendHandler( MOD_CMD_PKT(modFreqHz+10) ) } ],
  [ 'Modulation', 'Bulk Read', function() { sendHandler( [ CMDS['CMD_MOD_DATA_BULK_RUN'].address ] ) } ],
  [ 'Gimbal', 'Toggle Pan Power', function() { sendHandler( [ CMDS['CMD_GIMBAL_PAN_TOG'].address ] ) } ],
  [ 'Gimbal', 'Set Pan 0°', function() { sendHandler( SCAN_CMD_PKT( [ 0, gimbal.tilt ] ) ) } ],
  [ 'Gimbal', 'Set Pan 90°', function() { sendHandler( SCAN_CMD_PKT( [ 90, gimbal.tilt ] ) ) } ],
  [ 'Gimbal', 'Set Pan 180°', function() { sendHandler( SCAN_CMD_PKT( [ 180, gimbal.tilt ] ) ) } ],
  [ 'Gimbal', 'Pan -5.0°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan-5.0, gimbal.tilt+0.0 ] ) ) } ],
  [ 'Gimbal', 'Pan -0.15°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan-0.15, gimbal.tilt+0.0 ] ) ) } ],
  [ 'Gimbal', 'Pan +0.15°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+0.15, gimbal.tilt+0.0 ] ) ) } ],
  [ 'Gimbal', 'Pan +5.0°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+5.0, gimbal.tilt+0.0 ] ) ) } ],
  [ 'Gimbal', 'Toggle Tilt Power', function() { sendHandler( [ CMDS['CMD_GIMBAL_TILT_TOG'].address ] ) } ],
  [ 'Gimbal', 'Set Tilt 0°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan, 0 ] ) ) } ],
  [ 'Gimbal', 'Set Tilt 90°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan, 90 ] ) ) } ],
  [ 'Gimbal', 'Set Tilt 180°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan, 180 ] ) ) } ],
  [ 'Gimbal', 'Tilt -5.0°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+0.0, gimbal.tilt-5.0 ] ) ) } ],
  [ 'Gimbal', 'Tilt -0.15°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+0.0, gimbal.tilt-0.15 ] ) ) } ],
  [ 'Gimbal', 'Tilt +0.15°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+0.0, gimbal.tilt+0.15 ] ) ) } ],
  [ 'Gimbal', 'Tilt +5.0°', function() { sendHandler( SCAN_CMD_PKT( [ gimbal.pan+0.0, gimbal.tilt+5.0 ] ) ) } ],
  [ 'Gimbal', 'Get Pan', function() { sendHandler( [ CMDS['CMD_GIMBAL_PAN_ANG_GET'].address ] ) } ],
  [ 'Gimbal', 'Get Tilt', function() { sendHandler( [ CMDS['CMD_GIMBAL_TILT_ANG_GET'].address ] ) } ],
  [ 'Gimbal', 'Origin', function() { sendHandler( SCAN_CMD_PKT( [ 90, 90 ] ) ) } ],
  [ 'HID USB', 'Connect', hidConnect ],
  [ 'HID USB', 'Disconnect', hidDisconnect ],
  [ 'Debug', 'LED1', function() { sendHandler( [ CMDS['CMD_LED1_TOG'].address ] ) } ],
  [ 'Debug', 'LED2', function() { sendHandler( [ CMDS['CMD_LED2_TOG'].address ] ) } ],
  [ 'Debug', 'BTN1', function() { sendHandler( [ CMDS['CMD_BTN1_GET'].address ] ) } ],
  [ 'Debug', 'BTN2', function() { sendHandler( [ CMDS['CMD_BTN2_GET'].address ] ) } ],
  [ 'Debug', 'Value1', function() { sendHandler( [ CMDS['CMD_DBG_VAL1_GET'].address ] ) } ],
  [ 'Debug', 'Value2', function() { sendHandler( [ CMDS['CMD_DBG_VAL2_GET'].address ] ) } ],
]

// initalize underlying magic  ...

// ... behavior/dynamics of existing DOM elements
initBase()

// ... our DOM grid
var graphContainerDiv = document.getElementById( 'graphs' )
//var heatmapContainerDiv = document.getElementById( 'heatmaps' )
var controlsContainerDiv = document.getElementById( 'rightSidebarToolbox1' )
var terminalContainerDiv = document.getElementById( 'rightSidebarToolbox2' )
var otherContainerDiv = document.getElementById( 'rightSidebarToolbox3' )

// ... gimbal helper class
var gimbal = new Gimbal()

// variable to keep track of ongoing operations
var modFreqHz = null
var modBulkBuffer = null
var qpBulkBuffer = null
var qpBulkBufferCh1 = null
var qpBulkBufferCh2 = null
var qpBulkBufferCh3 = null
var qpBulkBufferCh4 = null

// inialize USB ...

// ... HID object
var hid = new Hid( receiveHandler, hidConnect, hidDisconnect )
hidConnect()

// ... flags for continuous operations of the MCU
var measureFlag = false
var scanFlag = false

// ... generic command packets for common tasks (use as building blocks
// for larger queries
var SCAN_CMD_PKT = ( function( panTilt=null ) {
  if ( panTilt != null )
    gimbal.set( panTilt )
  return [
    CMDS['CMD_GIMBAL_PAN_ANG_SET'].address,
      (Math.round(gimbal.panBytes)>>8)%256,
      (Math.round(gimbal.panBytes)>>0)%256,
    CMDS['CMD_GIMBAL_TILT_ANG_SET'].address,
      (Math.round(gimbal.tiltBytes)>>8)%256,
      (Math.round(gimbal.tiltBytes)>>0)%256,
    ]
} )
var MOD_CMD_PKT = ( function( rateHz ) { 
  modFreqHz = rateHz
  return [
    CMDS['CMD_MOD_FREQ_HZ_SET'].address,
      (Math.round(rateHz)>>24)%256,
      (Math.round(rateHz)>>16)%256,
      (Math.round(rateHz)>>8)%256,
      (Math.round(rateHz)>>0)%256,
    ] 
} )
var DEFAULT_MCU_PKT = ( function() { return [
  CMDS['CMD_QP_CH1_VSENSE_GET'].address,
  CMDS['CMD_QP_CH2_VSENSE_GET'].address,
  CMDS['CMD_QP_CH3_VSENSE_GET'].address,
  CMDS['CMD_QP_CH4_VSENSE_GET'].address,
  CMDS['CMD_LSR_LOAD_SWTICH_GET'].address,
  CMDS['CMD_LSR_VREF_VSENSE_GET'].address,
  CMDS['CMD_LSR_ISENSE_GET'].address,
  CMDS['CMD_LSR_GET'].address,
  CMDS['CMD_MOD_GET'].address,
  CMDS['CMD_MOD_FREQ_HZ_GET'].address,
  CMDS['CMD_MOD_FREQ_HZ_GET'].address,
  CMDS['CMD_MOD_SIG_LOCK_GET'].address,
  CMDS['CMD_GIMBAL_PAN_GET'].address,
  CMDS['CMD_GIMBAL_TILT_GET'].address,
  CMDS['CMD_GIMBAL_PAN_ANG_GET'].address,
  CMDS['CMD_GIMBAL_TILT_ANG_GET'].address,
  CMDS['CMD_LED1_GET'].address,
  CMDS['CMD_LED2_GET'].address,
  CMDS['CMD_BTN1_GET'].address,
  CMDS['CMD_BTN2_GET'].address,
  CMDS['CMD_DBG_VAL1_GET'].address,
  CMDS['CMD_DBG_VAL2_GET'].address,
] } )

// ... helper functions (most of which would have issues with scope)
var mcuPollerH = null
function stopPoller() {
  
  // try to cancel any existing MCU poller
  if ( mcuPollerH != null ) {
    window.clearInterval( mcuPollerH )
    mcuPollerH = null
  }
  
  return
}
function startPoller() {
  
  // try to cancel any existing MCU poller
  stopPoller()
  
  // now that we are connected to our MCU, we should set up a poller to
  // get information every so often (only if it is a resonable size and
  // we see that response packet will also be reasonably sized
  mcuPollerTxSize = 0
  mcuPollerRxSize = 0
  DEFAULT_MCU_PKT().forEach( function( address, idx1, arr ) {
    cmdKeys = Object.keys(CMDS)
    for ( var idx2=0; idx2<cmdKeys.length; idx2++ ) {
      if ( CMDS[cmdKeys[idx2]].address == address ) {
        mcuPollerTxSize += ( CMDS[cmdKeys[idx2]].txBytes + 1 )
        mcuPollerRxSize += ( CMDS[cmdKeys[idx2]].rxBytes + 1 )
        break
      }
    }
  } )
  if ( mcuPollerTxSize <= Hid.BUF_SIZE ) {
    mcuPollerRate = 100
    mcuPollerH = window.setInterval( function() { sendHandler( DEFAULT_MCU_PKT() ) }, mcuPollerRate )
    console.log( 'Polling MCU with ' + mcuPollerTxSize + '-byte packet, expecting to receive ' + mcuPollerRxSize + '-byte packet every ' + mcuPollerRate + 'ms.' )
  } else {
    console.log( 'Unable to start default MCU polling, projected TX packet size is ' + mcuPollerTxSize + ' bytes and projected RX packet size is ' + mcuPollerRxSize + ' bytes.' )
  }
  
  return
}
function hidConnect() {
  
  // try to connect to USB device, define callbacks to run if 
  // success/failure
  hid.connect( null, successFunc=function() {
    
    // set our status bar to show that device is connected
    setStatusBar( 'active' )
    
    // send an initial MCU poll to get useful updates to our interface
    sendHandler( DEFAULT_MCU_PKT() )
    
    // attempt to start default poller
    startPoller()
    
    // have graphs updated nicely and scroll appropriately
    startTickTock()
    
    return
    
  }, function() {
    
    // set our status bar to show that we were unable to connect
    setStatusBar( '' )
    
    return
  } )
  
  return
}
function hidDisconnect() { 
  
  // attempt to disconnect from USB device, define callbacks to
  // run if success/failure
  if ( hid.connection )
    hid.disconnect( hid.hidDeviceInfo.deviceId, function() {
      
      // cancel our MCU poller
      stopPoller()
      
      // stop graphs from updating nicely and scrolling
      stopTickTock()
      
      // set our status bar to show that device is disconnected
      setStatusBar( 'inactive' )
      
      return
    }, function() {
      
      // set our status bar to show that device was unable to be
      // disconnected
      setStatusBar( '' )
      
      return
    } )
  else
    setStatusBar( '' )
  
  return
}
function recolorizeHeatmaps() {
  heatmaps.forEach( function( heatmap ) { heatmap.fitRng() } )
}


// initialize any objects we want at the start ...

// ... graphs
var graphs = []
var newGraph = ( function( varargin ) {
  graphs.push( new Graph( graphContainerDiv, varargin ) )
  return graphs[graphs.length-1] 
} )
var graphVrefVSense = newGraph( ( {
  yLabelText: 'Laser Vref (V)',
  yRng: [ 0, 2 ],
} ) )
var graphLoadSwitchState = newGraph( ( {
  yLabelText: 'Load Switch State',
  fitYRngEvent: null,
  yRng: [ -0.1, 1.1 ],
} ) )
var graphISense = newGraph( ( {
  yLabelText: 'Laser Current (mA)',
  yRng: [ 0, 40 ],
} ) )
var graphLaserState = newGraph( ( {
  yLabelText: 'Laser State',
  fitYRngEvent: null,
  yRng: [ -0.1, 1.1 ],
} ) )
var graphDebugValue1 = newGraph( ( {
  yLabelText: 'Debug Value1',
  yRng: [ 0, 1 ],
} ) )
var graphDebugValue2 = newGraph( ( {
  yLabelText: 'Debug Value2',
  yRng: [ 0, 1 ],
} ) )
var graphPan = newGraph( ( {
  yLabelText: 'Pan Servo (°)',
  yRng: [ 0, 180 ],
} ) )
var graphTilt = newGraph( ( {
  yLabelText: 'Tilt Servo (°)',
  yRng: [ 0, 180 ],
} ) )
var graphCh1VSense = newGraph( ( {
  yLabelText: 'Channel 1 (mV)',
} ) )
var graphCh4VSense = newGraph( ( {
  yLabelText: 'Channel 4 (mV)',
} ) )
var graphCh2VSense = newGraph( ( {
  yLabelText: 'Channel 2 (mV)',
} ) )
var graphCh3VSense = newGraph( ( {
  yLabelText: 'Channel 3 (mV)',
} ) )


// ... heatmaps
var heatmaps = []
var newHeatmap = ( function( varargin ) {
  heatmaps.push( new Heatmap( heatmapContainerDiv, varargin ) )
  return heatmaps[heatmaps.length-1] 
} )
//var heatmapTObj1 = newHeatmap()

// ... terminal
var terminal = new Log( terminalContainerDiv )
terminal.terminal( onEnter=sendHandler, cmdHash=CMDS )

// ... controls
var controls = new Controls( controlsContainerDiv )
CONTROLS.forEach( function( control ) {
  controls.addControl( control[0], control[1], control[2] )
} )


// setup callback for directing gimbal based on interaction with 
// heatmap(s)
Heatmap.MOUSE_OVER_CB = function( data ) {
  // ** This currently only supports the first heatmap to use as reference.
  // A small rewrite is needed to figure out which heatmap was triggered.
  // compute pan and tilt values using a heatmap for reference...
  // ...and then send all at once, how nice!
  gimbal.set( gimbal._locToPanTilt( data.loc, heatmaps[0] ) )
  sendHandler( SCAN_CMD_PKT() )
  
  return
}


// at this point everything should look okay, hide the loading curtain
hideCurtain()

// and go ahead and open up our sidebar for quick access
document.getElementById( 'rightSidebarMenuButton' ).click()
// and set menu button to currect state
window.setTimeout( function() { hamburger( 'un' ) }, 1000 )

function startTickTock() {
  
  // start ticking (only if graph already isn't ticking)
  graphs.forEach( function( graph ) {
    if ( !graph.tickTockRunning )
      graph.tickTock( false, 1500, 'bounce' )
  } )
  
  return
}

function stopTickTock() {
  
  // stop ticking (only if graph is already ticking)
  graphs.forEach( function( graph ) {
    if ( graph.tickTockRunning )
      graph.tickTockStop = true
  } )
  
  return
}

function sendHandler( dataArr, logId=null ) {
  // handler function that can encode and send data to the device
  
  // make sure are array isn't too big
  if ( dataArr.length > Hid.BUF_SIZE ) {
    console.log( 'Data array to send exceeds buffer size.' )
    return
  }
  
  // take data and store it in a buffer
  var bytes = new Uint8Array( Hid.BUF_SIZE )
  dataArr.forEach( function( val, idx ) { 
    if ( typeof( val ) == 'object' ) {
      console.log( 'You should not be sending objects to send handler. Like this one...' )
      console.log( val )
    }
    bytes[idx] = val
  } )
  
  // send data over usb
  //var id = terminal.addPersistent( 'Sending...' )
  var waitForCon = window.setInterval( function() {
    
    // wait here till our connection is good
    if ( hid.connection ) {
      
      clearInterval( waitForCon )
      
      chrome.hid.send( hid.connection.connectionId, 0, bytes.buffer, function() {
      
        // first check chrome for runtime errors (i.e. transfer errors)
        if ( chrome.runtime.lastError )
          console.log( chrome.runtime.lastError )
        
        // assume success at this point
        //id.innerHTML = id.innerHTML + 'Sent. '
        
      } ) 
    
    }
    
    return
  }, 100 )
  
  return
}

function receiveHandler( dataBuf ) {
  // handler function that can decode data from the device
  
  // quick conversion to make data usable
  var dataArr = new Uint8Array( Hid.BUF_SIZE )
  dataArr = new Uint8Array( dataBuf )
  
  // don't trust sender, check data to see if it is the right size
  if ( dataArr.length !== Hid.BUF_SIZE ) {
    console.log( 'Data array received is not the expected size.' )
    return
  }
  
  var idx = 0
  while ( idx<dataArr.length ) {
    
    // each data packet is (one byte plus number of data bytes) long
    // header --> data(msb) --> ... --> data(lsb)
    var headerByte = dataArr[idx++]
    function getDataBytes( cmdId ) {
      // get data from packet
      var dataBytes = []
      for ( var idx2=0; idx2<CMDS[cmdId].rxBytes; idx2++ ) 
        dataBytes[idx2] = dataArr[idx++] 
      //console.log( headerByte + ': ' + dataBytes )
      return dataBytes
    }
    
    // check to see if we still have data left
    if ( !headerByte )
      return
    
    // this is the heart of the rx handler...
    // the concept is simple, each packet we receive from the host should
    // have sent the same identifier header, we can use this header to 
    // determine what to do with the following data
    switch ( headerByte ) {
      
      case CMDS['CMD_GIMBAL_PAN_GET'].address:
      case CMDS['CMD_GIMBAL_PAN_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_GIMBAL_PAN_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphGimbalPanState !== 'undefined' )
          graphGimbalPanState.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Gimbal"]["Toggle Pan Power"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_GIMBAL_TILT_GET'].address:
      case CMDS['CMD_GIMBAL_TILT_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_GIMBAL_TILT_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphGimbalTiltState !== 'undefined' )
          graphGimbalTiltState.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Gimbal"]["Toggle Tilt Power"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_GIMBAL_PAN_ANG_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_GIMBAL_PAN_ANG_GET' )
        
        // build up 16-bit register contents
        var gimbalReg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate angle from knowledge of transmione of angles
        var angle = gimbalReg * 360 / ((1<<16)-1)
        
        // update gimbal object with new angle
        gimbal.set( [ angle, gimbal.tilt ] )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphPan !== 'undefined' )
          graphPan.addPoint( angle )
        //else
          controls.controlsByGroup["Gimbal"]["Get Pan"].querySelector( '#value' ).innerHTML = " - " + angle.toFixed(3) + "&deg;"
        
        break
        
      case CMDS['CMD_GIMBAL_TILT_ANG_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_GIMBAL_TILT_ANG_GET' )
        
        // build up 16-bit register contents
        var gimbalReg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate angle from knowledge of transmione of angles
        var angle = gimbalReg * 360 / ((1<<16)-1)
        
        // update gimbal object with new angle
        gimbal.set( [ gimbal.pan, angle ] )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphTilt !== 'undefined' )
          graphTilt.addPoint( angle )
        //else
          controls.controlsByGroup["Gimbal"]["Get Tilt"].querySelector( '#value' ).innerHTML = " - " + angle.toFixed(3) + "&deg;"
        
        break
      
    case CMDS['CMD_QP_CH1_VSENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_QP_CH1_VSENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        adc = reg * 3.3 / ((1<<10)-1)
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphCh1VSense !== 'undefined' )
          graphCh1VSense.addPoint( (adc*1000) )
        //else
          controls.controlsByGroup["Quadrant Photodiode"]["Channel 1"].querySelector( '#value' ).innerHTML = " (" + (reg) + ")"
        
        break
      
    case CMDS['CMD_QP_CH2_VSENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_QP_CH2_VSENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        adc = reg * 3.3 / ((1<<10)-1)
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphCh2VSense !== 'undefined' )
          graphCh2VSense.addPoint( (adc*1000) )
        //else
          controls.controlsByGroup["Quadrant Photodiode"]["Channel 2"].querySelector( '#value' ).innerHTML = " (" + (reg) + ")"
        
        break
      
      case CMDS['CMD_QP_CH3_VSENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_QP_CH3_VSENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        adc = reg * 3.3 / ((1<<10)-1)
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if (graphCh3VSense)
          graphCh3VSense.addPoint( (adc*1000) )
        //else
          controls.controlsByGroup["Quadrant Photodiode"]["Channel 3"].querySelector( '#value' ).innerHTML = " (" + (reg) + ")"
        
        break
      
      case CMDS['CMD_QP_CH4_VSENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_QP_CH4_VSENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        adc = reg * 3.3 / ((1<<10)-1)
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphCh4VSense !== 'undefined' )
          graphCh4VSense.addPoint( (adc*1000) )
        //else
          controls.controlsByGroup["Quadrant Photodiode"]["Channel 4"].querySelector( '#value' ).innerHTML = " (" + (reg) + ")"
        
        break
      
      case CMDS['CMD_QP_ALL_BULK_RUN'].address:
        
        // stop default poller since this is a bulk activity
        stopPoller()
        
        // graphically update value field in control
        controls.controlsByGroup["Quadrant Photodiode"]["Bulk Read"].querySelector( '#value' ).innerHTML = ' - ' + 'Reading ADCs...'
        
        // clear buffers holding ADC information
        qpBulkBuffer = []
        qpBulkBufferCh1 = []
        qpBulkBufferCh2 = []
        qpBulkBufferCh3 = []
        qpBulkBufferCh4 = []
        
        // trigger request to retrieve data we just made
        sendHandler( [ CMDS['CMD_QP_ALL_BULK_GET'].address ] )
        
        break
      
      case CMDS['CMD_QP_ALL_BULK_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_QP_ALL_BULK_GET' )
        
        // add bytes to array buffer
        for ( var count=0; count<CMDS['CMD_QP_ALL_BULK_GET'].rxBytes; count+=2 )
          qpBulkBuffer.push( bytesToUnsignedInt( dataBytes.slice(count,count+2) ) )
        
        // trim data array if too large
        if ( qpBulkBuffer.length > 4096 )
          qpBulkBuffer.splice( 4096, 4096 )
        
        // graphically update value field in control
        controls.controlsByGroup["Quadrant Photodiode"]["Bulk Read"].querySelector( '#value' ).innerHTML = ' - ' + 'Retrieving Data...' + qpBulkBuffer.length + '/4096'
        
        if ( qpBulkBuffer.length < 4096 ) {
          // send another request for more data if we don't have it all yet
          sendHandler( [ CMDS['CMD_QP_ALL_BULK_GET'].address ] )
        } else {
          // format the data into seperate arrays while also forming csv
          // formatted array
          var csvArr = []
          for ( var count=0; count<qpBulkBuffer.length; count++ ) {
            // calculate voltage value assuming 10-bit ADC with 3.3V reference
            var value = qpBulkBuffer[count] * 3.3 / ((1<<10)-1)
            
            // place into correct array
            switch ( (count+1) % 4 ) {
              case 0:
                qpBulkBufferCh4.push( value )
                csvArr.push( [
                  qpBulkBufferCh1[qpBulkBufferCh1.length-1],
                  qpBulkBufferCh2[qpBulkBufferCh2.length-1],
                  qpBulkBufferCh3[qpBulkBufferCh3.length-1],
                  qpBulkBufferCh4[qpBulkBufferCh4.length-1],
                ] )
                break
              case 1:
                qpBulkBufferCh3.push( value )
                break
              case 2:
                qpBulkBufferCh2.push( value )
                break
              case 3:
                qpBulkBufferCh1.push( value )
                break
            }
          }
          
          // create csv file with data
          var lineArray = []
          csvArr.forEach( function ( infoArray, index ) {
              var line = infoArray.join( ',' )
              lineArray.push( index == 0 ? "data:text/csv;charset=utf-8," + line : line )
          } )
          var csvContent = lineArray.join( '\n' )
          var encodedUri = encodeURI( csvContent )
          var link = document.createElement( 'a' )
          link.setAttribute( 'href', encodedUri )
          var newDate = new Date()
          var timestamp = newDate.getFullYear()+'_'+parseInt(newDate.getMonth()+1)+'_'+newDate.getDate()+'-'+newDate.getHours()+'_'+newDate.getMinutes()+'_'+newDate.getSeconds()
          link.setAttribute( 'download', timestamp + '.csv' )
          document.body.appendChild( link )
          link.click()
          
          // restart default activity now that bulk action is done
          startPoller()
        }
        
        break
      
      case CMDS['CMD_LSR_LOAD_SWTICH_GET'].address:
      case CMDS['CMD_LSR_LOAD_SWTICH_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LSR_LOAD_SWTICH_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphLoadSwitchState !== 'undefined' )
          graphLoadSwitchState.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Laser"]["Load Switch"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
      
      case CMDS['CMD_LSR_VREF_VSENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LSR_VREF_VSENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        vref = reg * 3.3 / ((1<<10)-1)
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphVrefVSense !== 'undefined' )
          graphVrefVSense.addPoint( vref )
        //else
          controls.controlsByGroup["Laser"]["Vref Voltage Sense"].querySelector( '#value' ).innerHTML = " (" + vref.toFixed(3) + ")"
        
        break
        
      case CMDS['CMD_LSR_ISENSE_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LSR_ISENSE_GET' )
        
        // build up 16-bit register contents
        var reg = bytesToUnsignedInt( dataBytes.slice(0,2) )
        
        // calculate voltage value assuming 10-bit ADC with 3.3V reference
        // and a 0.82 sense resistor with a desired unit of mA
        iSense = reg * 3.3 / ((1<<10)-1) / 0.82 * 1000
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphISense !== 'undefined' )
          graphISense.addPoint( iSense )
        //else
          controls.controlsByGroup["Laser"]["Current Sense"].querySelector( '#value' ).innerHTML = " (" + vref.toFixed(3) + ")"
        
        break
        
      case CMDS['CMD_LSR_GET'].address:
      case CMDS['CMD_LSR_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LSR_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphLaserState !== 'undefined' )
          graphLaserState.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Laser"]["Toggle Laser"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_MOD_TOG'].address:
      case CMDS['CMD_MOD_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModState !== 'undefined' )
          graphModState.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Modulation"]["Modulate"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
      
      case CMDS['CMD_MOD_FREQ_HZ_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_FREQ_HZ_GET' )
        
        // update modulation rate variable for tracking
        modFreqHz = bytesToUnsignedLong(dataBytes.slice(0,4))
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModRate !== 'undefined' )
          graphModRate.addPoint( modFreqHz )
        //else
          controls.controlsByGroup["Modulation"]["Current Rate"].querySelector( '#value' ).innerHTML = " (" + modFreqHz + ")"
        
        break
      
      case CMDS['CMD_MOD_SIG_LOCK_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_SIG_LOCK_GET' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModSigLockState !== 'undefined' )
          graphModSigLockState.addPoint( dataBytes[0]  )
        //else
          controls.controlsByGroup["Modulation"]["Lock State"].querySelector( '#value' ).innerHTML = " (Lock=" + dataBytes[0] + ")"
        
        break
      
      case CMDS['CMD_MOD_DATA_BULK_RUN'].address:
        
        // stop default poller since this is a bulk activity
        stopPoller()
        
        // graphically update value field in control
        controls.controlsByGroup["Modulation"]["Bulk Read"].querySelector( '#value' ).innerHTML = ' - ' + 'Reading ADCs...'
        
        // clear buffer holding ADC information
        modBulkBuffer = []
        
        // trigger request to retrieve data we just made
        sendHandler( [ CMDS['CMD_MOD_DATA_BULK_GET'].address ] )
        
        break
      
      case CMDS['CMD_MOD_DATA_BULK_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_DATA_BULK_GET' )
        
        // add bytes to array buffer
        for ( var count=0; count<CMDS['CMD_MOD_DATA_BULK_GET'].rxBytes; count+=2 )
          modBulkBuffer.push( bytesToUnsignedInt( dataBytes.slice(count,count+2) ) )
        
        // trim data array if too larger
        if ( modBulkBuffer.length > 256 )
          modBulkBuffer.splice( 256, 256 )
        
        // graphically update value field in control
        controls.controlsByGroup["Modulation"]["Bulk Read"].querySelector( '#value' ).innerHTML = ' - ' + 'Retrieving Data...' + modBulkBuffer.length + '/256'
        
        if ( modBulkBuffer.length < 256 ) {
          // send another request for more data if we don't have it all yet
          sendHandler( [ CMDS['CMD_MOD_DATA_BULK_GET'].address ] )
        } else {
          // format the data into seperate arrays while also forming csv
          // formatted array
          var csvArr = []
          for ( var count=0; count<modBulkBuffer.length; count++ ) {
            // calculate voltage value assuming 10-bit ADC with 3.3V reference
            var value = modBulkBuffer[count]// * 3.3 / ((1<<10)-1)
            
            // place into correct array
            csvArr.push( [value] )
          }
          
          // create csv file with data
          var lineArray = []
          csvArr.forEach( function ( infoArray, index ) {
              var line = infoArray.join( ',' )
              lineArray.push( index == 0 ? "data:text/csv;charset=utf-8," + line : line )
          } )
          var csvContent = lineArray.join( '\n' )
          var encodedUri = encodeURI( csvContent )
          var link = document.createElement( 'a' )
          link.setAttribute( 'href', encodedUri )
          var newDate = new Date()
          var timestamp = newDate.getFullYear()+'_'+parseInt(newDate.getMonth()+1)+'_'+newDate.getDate()+'-'+newDate.getHours()+'_'+newDate.getMinutes()+'_'+newDate.getSeconds()
          link.setAttribute( 'download', timestamp + '.csv' )
          document.body.appendChild( link )
          link.click()
          
          // restart default activity now that bulk action is done
          startPoller()
        }
        
        break
      
      case CMDS['CMD_LED1_GET'].address:
      case CMDS['CMD_LED1_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LED1_GET' )
        
        // graphically update value field in control
        controls.controlsByGroup["Debug"]["LED1"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
      
      case CMDS['CMD_LED2_GET'].address:
      case CMDS['CMD_LED2_TOG'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_LED2_GET' )
        
        // graphically update value field in control
        controls.controlsByGroup["Debug"]["LED2"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_BTN1_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_BTN1_GET' )
        
        // graphically update value field in control
        controls.controlsByGroup["Debug"]["BTN1"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_BTN2_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_BTN2_GET' )
        
        // graphically update value field in control
        controls.controlsByGroup["Debug"]["BTN2"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
      
      case CMDS['CMD_DBG_VAL1_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_VAL1_GET' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugValue1 !== 'undefined' )
          graphDebugValue1.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Value1"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        break
      
      case CMDS['CMD_DBG_VAL2_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_VAL2_GET' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugValue2 !== 'undefined' )
          graphDebugValue2.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Value2"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        
        break
      
      default:
        
        // don't recognize this header identifier, assume that we simply
        // do not care about handling this case, i.e. it may be a return
        // for a command we sent that we don't need confirmation for
        //console.log( 'Unrecognized header in received packet: ' + headerByte )
        
    }
  
  }
  
  //temps.push( ((data[0]<<8)+data[1])/50 - 273.15 )
  //addPoint( graphObjTOBJ1, temps[temps.length-1] )
  //addHeatmapHex( heatmapObjTOBJ1, temps[temps.length-1] )
  // temps[temps.length-1]
  
  return
}

function bytesToUnsignedInt( bytes ) {
  return ( ((bytes[0]<<8)) + 
           ((bytes[1]<<0)) )
}
function bytesToUnsignedLong( bytes ) {
  return ( ((bytes[0]<<24)>>>0) + 
           ((bytes[1]<<16)>>>0) + 
           ((bytes[2]<<8)>>>0) + 
           ((bytes[3]<<0)>>>0) )
}

// experimentk for fast data collectoin
var timeTH
function timeT() {
  timeTH=window.setInterval( function() { sendHandler( [
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  CMDS['CMD_TOBJ1_GET'].address,
  ] ) }, 10 )
  window.setTimeout( function() {
    graphTObj1.tickTock()
    window.clearInterval(timeTH)
  }, 110 )
  return
}

function tObjProcessFunc( dataBytes, graphs, heatmaps, coords=null ) {
  
  // build up 16-bit register contents
  var tempReg = bytesToUnsignedInt( dataBytes.slice(0,2) )
  
  // convert register to degrees C
  var temp = tempReg/50 - 273.15
  
  // get time information
  var unixTimestamp = bytesToUnsignedLong( dataBytes.slice(6,10) )
  var ms = bytesToUnsignedInt( dataBytes.slice(10,12) )
  
  // handle whether we think on board time information is good
  if ( hid.onBoardClocking )
    var timeWithMs = unixTimestamp + (ms/1000)
  else
    var timeWithMs = Date.now()// + (ms/1000)
  
  // add point to graph object(s)
  graphs.forEach( function( graph) { graph.addPoint( temp ) } )
  
  heatmaps.forEach( function( heatmap ) { 
    
    // use either local pan/tilt values or ones sent by MCU
    if ( coords ) {
      panReg = bytesToUnsignedInt( dataBytes.slice(2,4) )
      pan = panReg / ((1<<16)-1)* 360
      tiltReg = bytesToUnsignedInt( dataBytes.slice(4,6 ) )
      tilt = tiltReg / ((1<<16)-1)* 360
    }
    
    // compute details needed for heatmap placement
    if ( tilt == 0 )
      locTilt = 0
    else
      locTilt = Math.round( (heatmap.dim[0]-1) * ( tilt - gimbal.tiltRngLim[0] ) / ( gimbal.tiltRngLim[1] - gimbal.tiltRngLim[0] ) )
    if ( pan == 0 ) {
      locPan = 0
    } else {
      if ( locTilt % 2 )
        locPan = Math.round( (heatmap.dim[1]-0.5) * ( pan - gimbal.panRngLim[0] ) / ( gimbal.panRngLim[1] - gimbal.panRngLim[0] ) )
      else
        locPan = Math.round( (heatmap.dim[1]-0.5) * ( pan - gimbal.panRngLim[0] ) / ( gimbal.panRngLim[1] - gimbal.panRngLim[0] ) - 0.5 )
    }
    
    // add point to heatmap object(s)
    heatmap.colorize( [locTilt,locPan], temp )
  } )
  
  return
}
