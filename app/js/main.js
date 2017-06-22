var CMDS = {
  // 0x0*
  // 0x1* and 0x2* debugging commands
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
  'CMD_DBG_VAL3_GET': {
    address:  0x1A,
    rxBytes:  4,
    txBytes:  0,
    },
  'CMD_DBG_VAL4_GET': {
    address:  0x1B,
    rxBytes:  4,
    txBytes:  0,
    },
  'CMD_DBG_INC1': {
     address:  0x1C,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_DEC1': {
     address:  0x1D,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_INC2': {
     address:  0x1E,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_DEC2': {
     address:  0x1F,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_INC3': {
     address:  0x20,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_DEC3': {
     address:  0x21,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_INC4': {
     address:  0x22,
     rxBytes:  4,
     txBytes:  0,
     },
  'CMD_DBG_DEC4': {
     address:  0x23,
     rxBytes:  4,
     txBytes:  0,
     },
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
  // 0x8* and 0x9* modulation activities
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
  'CMD_MOD_HICCUP_NS_GET': {
    address:  0x89,
    rxBytes:  4,
    txBytes:  0,
  },
  'CMD_MOD_HICCUP_NS_SET': {
    address:  0x8A,
    rxBytes:  0,
    txBytes:  4,
  },
  'CMD_MOD_TOG_ALIGN_ENB': {
    address:  0x8B,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_SET_ALIGN_ENB': {
    address:  0x8C,
    rxBytes:  0,
    txBytes:  1,
  },
  'CMD_MOD_GET_ALIGN_ENB': {
    address:  0x8D,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_TOG_ALIGN_RHO_ENB': {
    address:  0x8E,
    rxBytes:  1,
    txBytes:  0,
  },
  'CMD_MOD_SET_ALIGN_RHO_ENB': {
    address:  0x8F,
    rxBytes:  0,
    txBytes:  1,
  },
  'CMD_MOD_GET_ALIGN_RHO_ENB': {
    address:  0x90,
    rxBytes:  1,
    txBytes:  0,
  },
  // 0xA*
  // 0xB*
  // 0xC*
  // 0xD*
  // 0xE*
  // 0xF* identification commands
  'CMD_ID_MASTER_GET': {
    address:  0xF0,
    rxBytes:  1,
    txBytes:  0,
  },
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
  [ 'Modulation', '+10Hz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz+10) ) } ],
  [ 'Modulation', '+100Hz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz+100) ) } ],
  [ 'Modulation', '+1kHz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz+1000) ) } ],
  [ 'Modulation', 'Current Freq', function() { sendHandler( [ CMDS['CMD_MOD_FREQ_HZ_GET'].address ] ) } ],
  [ 'Modulation', '-10Hz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz-10) ) } ],
  [ 'Modulation', '-100Hz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz-100) ) } ],
  [ 'Modulation', '-1kHz', function() { sendHandler( MOD_FREQ_HZ_CMD_PKT(modFreqHz-1000) ) } ],
  [ 'Modulation', 'Lock State', function() { sendHandler( [ CMDS['CMD_MOD_SIG_LOCK_GET'].address ] ) } ],
  [ 'Modulation', '+10ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs+10) ) } ],
  [ 'Modulation', '+100ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs+100) ) } ],
  [ 'Modulation', '+1000ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs+1000) ) } ],
  [ 'Modulation', 'Current Hiccup', function() { sendHandler( [ CMDS['CMD_MOD_HICCUP_NS_GET'].address ] ) } ],
  [ 'Modulation', '-10ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs-10) ) } ],
  [ 'Modulation', '-100ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs-100) ) } ],
  [ 'Modulation', '-1000ns', function() { sendHandler( MOD_HICCUP_NS_CMD_PKT(modHiccupNs-1000) ) } ],
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
  [ 'Gimbal', 'Get Pan/Tilt', function() { sendHandler( [ CMDS['CMD_GIMBAL_PAN_ANG_GET'].address, CMDS['CMD_GIMBAL_TILT_ANG_GET'].address ] ) } ],
  [ 'Gimbal', 'Origin', function() { sendHandler( SCAN_CMD_PKT( [ 90, 90 ] ) ) } ],
  [ 'Gimbal', 'Toggle Alignment', function() { sendHandler( [ CMDS['CMD_MOD_TOG_ALIGN_ENB'].address ] ) } ],
  [ 'Gimbal', 'Toggle Momentum', function() { sendHandler( [ CMDS['CMD_MOD_TOG_ALIGN_RHO_ENB'].address ] ) } ],
  [ 'HID USB', 'Connect', hidConnect ],
  [ 'HID USB', 'Disconnect', hidDisconnect ],
  [ 'Debug', 'LED1', function() { sendHandler( [ CMDS['CMD_LED1_TOG'].address ] ) } ],
  [ 'Debug', 'LED2', function() { sendHandler( [ CMDS['CMD_LED2_TOG'].address ] ) } ],
  [ 'Debug', 'BTN1', function() { sendHandler( [ CMDS['CMD_BTN1_GET'].address ] ) } ],
  [ 'Debug', 'BTN2', function() { sendHandler( [ CMDS['CMD_BTN2_GET'].address ] ) } ],
  [ 'Debug', 'Value1', function() { sendHandler( [ CMDS['CMD_DBG_VAL1_GET'].address ] ) } ],
  [ 'Debug', 'Value2', function() { sendHandler( [ CMDS['CMD_DBG_VAL2_GET'].address ] ) } ],
  [ 'Debug', 'Value3', function() { sendHandler( [ CMDS['CMD_DBG_VAL3_GET'].address ] ) } ],
  [ 'Debug', 'Value4', function() { sendHandler( [ CMDS['CMD_DBG_VAL4_GET'].address ] ) } ],
  [ 'Debug', 'Increment1', function() { sendHandler( [ CMDS['CMD_DBG_INC1'].address ] ) } ], 
  [ 'Debug', 'Increment2', function() { sendHandler( [ CMDS['CMD_DBG_INC2'].address ] ) } ], 
  [ 'Debug', 'Increment3', function() { sendHandler( [ CMDS['CMD_DBG_INC3'].address ] ) } ], 
  [ 'Debug', 'Increment4', function() { sendHandler( [ CMDS['CMD_DBG_INC4'].address ] ) } ], 
  [ 'Debug', 'Decrement1', function() { sendHandler( [ CMDS['CMD_DBG_DEC1'].address ] ) } ], 
  [ 'Debug', 'Decrement2', function() { sendHandler( [ CMDS['CMD_DBG_DEC2'].address ] ) } ], 
  [ 'Debug', 'Decrement3', function() { sendHandler( [ CMDS['CMD_DBG_DEC3'].address ] ) } ], 
  [ 'Debug', 'Decrement4', function() { sendHandler( [ CMDS['CMD_DBG_DEC4'].address ] ) } ], 
]

// initalize underlying magic  ...

// ... behavior/dynamics of existing DOM elements
initBase()

// ... our DOM grid
var graphContainerDiv = document.getElementById( 'graphs' )
var deviceContainerDiv = document.getElementById( 'rightSidebarToolbox1' )
var controlsContainerDiv = document.getElementById( 'rightSidebarToolbox2' )
var terminalContainerDiv = document.getElementById( 'rightSidebarToolbox3' )
var otherContainerDiv = document.getElementById( 'rightSidebarToolbox4' )

// ... gimbal helper class
var gimbal = new Gimbal()

// variable to keep track of ongoing operations
var modFreqHz = null
var modHiccupNs = null
var modBulkBuffer = null
var qpBulkBuffer = null

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
var MOD_FREQ_HZ_CMD_PKT = ( function( rateHz ) { 
  modFreqHz = rateHz
  return [
    CMDS['CMD_MOD_FREQ_HZ_SET'].address,
      (Math.round(rateHz)>>24)%256,
      (Math.round(rateHz)>>16)%256,
      (Math.round(rateHz)>>8)%256,
      (Math.round(rateHz)>>0)%256,
    ] 
} )
var MOD_HICCUP_NS_CMD_PKT = ( function( ns ) { 
  modHiccupNs = ns
  return [
    CMDS['CMD_MOD_HICCUP_NS_SET'].address,
      (Math.round(ns)>>24)%256,
      (Math.round(ns)>>16)%256,
      (Math.round(ns)>>8)%256,
      (Math.round(ns)>>0)%256,
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
  CMDS['CMD_MOD_HICCUP_NS_GET'].address,
  CMDS['CMD_MOD_GET_ALIGN_ENB'].address,
  CMDS['CMD_MOD_GET_ALIGN_RHO_ENB'].address,
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
  CMDS['CMD_DBG_VAL3_GET'].address,
  CMDS['CMD_DBG_VAL4_GET'].address,
  CMDS['CMD_ID_MASTER_GET'].address,
] } )

// ... helper functions (most of which would have issues with scope)
var mcuPollerH = null
var packets = null
var packetsSendIdx = 0
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
  mcuPollerTxSize = [ 0 ]
  mcuPollerRxSize = [ 0 ]
  packets = [[]]
  packetIdx = 0
  DEFAULT_MCU_PKT().forEach( function( address, idx1, arr ) {
    cmdKeys = Object.keys(CMDS)
    for ( var idx2=0; idx2<cmdKeys.length; idx2++ ) {
      if ( CMDS[cmdKeys[idx2]].address == address ) {
        if ( ( ( mcuPollerTxSize[packetIdx] + CMDS[cmdKeys[idx2]].txBytes + 1 ) > Hid.BUF_SIZE ) ||
             ( ( mcuPollerRxSize[packetIdx] + CMDS[cmdKeys[idx2]].rxBytes + 1 ) > Hid.BUF_SIZE ) ) {
          packets.push( [] )
          mcuPollerTxSize.push( 0 )
          mcuPollerRxSize.push( 0 )
          packetIdx++
        }
        packets[packetIdx].push( CMDS[cmdKeys[idx2]].address )
        mcuPollerTxSize[packetIdx] += ( CMDS[cmdKeys[idx2]].txBytes + 1 )
        mcuPollerRxSize[packetIdx] += ( CMDS[cmdKeys[idx2]].rxBytes + 1 )
        
        break
      }
    }
  } )
  if ( mcuPollerTxSize.every( function( val ) { return val <= Hid.BUF_SIZE } ) &&
       mcuPollerRxSize.every( function( val ) { return val <= Hid.BUF_SIZE } ) ) {
    mcuPollerRate = 125
    mcuPollerH = window.setInterval( function() { 
      sendHandler( packets[packetsSendIdx] )
      if ( (packetsSendIdx + 1) == packets.length )
        packetsSendIdx = 0
      else
        packetsSendIdx++
    }, mcuPollerRate )
    console.log( 'Polling MCU with ' + (packets.length ) + ' ' + Hid.BUF_SIZE + '-byte packet every ' + mcuPollerRate + 'ms.' )
  } else {
    console.log( 'Unable to start default MCU polling, projected TX packet sizes are ' + mcuPollerTxSize + ' bytes and projected RX packet sizes are ' + mcuPollerRxSize + ' bytes.' )
  }
  
  return
}
function hidConnect() {
  
  // try to connect to USB device, define callbacks to run if 
  // success/failure
  hid.connect( null, successFunc=function() {
    
    // set our status bar to show that device is connected
    setStatusBar( 'active' )
    
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
var graphISense = newGraph( ( {
  yLabelText: 'Laser Current (mA)',
  yRng: [ 0, 40 ],
} ) )
var graphDebugValue1 = newGraph( ( {
  yLabelText: 'Debug Value 1',
  yRng: [ 0, 1 ],
} ) )
var graphDebugValue2 = newGraph( ( {
  yLabelText: 'Debug Value 2',
  yRng: [ 0, 1 ],
} ) )
var graphDebugValue3 = newGraph( ( {
  yLabelText: 'Debug Value 3',
  yRng: [ 0, 1 ],
} ) )
var graphDebugValue4 = newGraph( ( {
  yLabelText: 'Debug Value 4',
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
var graphCh3VSense = newGraph( ( {
  yLabelText: 'Channel 3 (mV)',
} ) )
var graphCh2VSense = newGraph( ( {
  yLabelText: 'Channel 2 (mV)',
} ) )
var graphCh4VSense = newGraph( ( {
  yLabelText: 'Channel 4 (mV)',
} ) )
var graphCh1VSense = newGraph( ( {
  yLabelText: 'Channel 1 (mV)',
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
//document.getElementById( 'rightSidebarMenuButton' ).click()
controlsContainerDiv.children[0].click()
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
          var prev = controls.controlsByGroup["Gimbal"]["Get Pan/Tilt"].querySelector( '#value' ).innerHTML
          controls.controlsByGroup["Gimbal"]["Get Pan/Tilt"].querySelector( '#value' ).innerHTML = " (" + angle.toFixed(3) + "&deg;," + prev.substring(11,99)
        
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
          var prev = controls.controlsByGroup["Gimbal"]["Get Pan/Tilt"].querySelector( '#value' ).innerHTML
          controls.controlsByGroup["Gimbal"]["Get Pan/Tilt"].querySelector( '#value' ).innerHTML =  prev.substring(0,10) + angle.toFixed(3) + "&deg;)"
        
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
          exportCsv( formatArr( qpBulkBuffer, 4, 1 ) )
          
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
          controls.controlsByGroup["Laser"]["Current Sense"].querySelector( '#value' ).innerHTML = " (" + iSense.toFixed(2) + ")"
        
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
          controls.controlsByGroup["Modulation"]["Current Freq"].querySelector( '#value' ).innerHTML = " (" + modFreqHz + ")"
        
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
          // formatted array and export
          exportCsv( formatArr( modBulkBuffer, 1, 1 ) )
          
          
          // restart default activity now that bulk action is done
          startPoller()
        }
        
        break
      
      case CMDS['CMD_MOD_HICCUP_NS_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_HICCUP_NS_GET' )
        
        // update modulation rate variable for tracking
        modHiccupNs = bytesToUnsignedLong(dataBytes.slice(0,4))
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModHiccupNs !== 'undefined' )
          graphModHiccupNs.addPoint( modHiccupNs )
        //else
          controls.controlsByGroup["Modulation"]["Current Hiccup"].querySelector( '#value' ).innerHTML = " (" + modHiccupNs + ")"
        
        break
      
      case CMDS['CMD_MOD_GET_ALIGN_ENB'].address:
      case CMDS['CMD_MOD_TOG_ALIGN_ENB'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_GET_ALIGN_ENB' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModAlignEnb !== 'undefined' )
          graphModAlignEnb.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Gimbal"]["Toggle Alignment"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
        break
        
      case CMDS['CMD_MOD_GET_ALIGN_RHO_ENB'].address:
      case CMDS['CMD_MOD_TOG_ALIGN_RHO_ENB'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_MOD_GET_ALIGN_RHO_ENB' )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphModAlignRhoEnb !== 'undefined' )
          graphModAlignRhoEnb.addPoint( dataBytes[0] )
        //else
          controls.controlsByGroup["Gimbal"]["Toggle Momentum"].querySelector( '#value' ).innerHTML = " (" + dataBytes[0] + ")"
        
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
      
      case CMDS['CMD_ID_MASTER_GET'].address:
      
      // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_ID_MASTER_GET' )
        
        // graphically update value field in GUI
        if ( dataBytes[0] )
          deviceContainerDiv.children[0].innerHTML = 'Device ID: MASTER'
        else
          deviceContainerDiv.children[0].innerHTML = 'Device ID: SLAVE'
        
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
      
      case CMDS['CMD_DBG_VAL3_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_VAL3_GET' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugValue3 !== 'undefined' )
          graphDebugValue3.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Value3"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        break
      
      case CMDS['CMD_DBG_VAL4_GET'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_VAL4_GET' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugValue4 !== 'undefined' )
          graphDebugValue4.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Value4"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        break
      
      case CMDS['CMD_DBG_INC1'].address:
      case CMDS['CMD_DBG_DEC1'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_INC1' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugInc1 !== 'undefined' )
          graphDebugInc1.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Increment1"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        controls.controlsByGroup["Debug"]["Decrement1"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        
        break
        
      case CMDS['CMD_DBG_INC2'].address:
      case CMDS['CMD_DBG_DEC2'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_INC2' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        controls.controlsByGroup["Debug"]["Increment2"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        controls.controlsByGroup["Debug"]["Decrement2"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        break
        
      case CMDS['CMD_DBG_INC3'].address:
      case CMDS['CMD_DBG_DEC3'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_INC3' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        controls.controlsByGroup["Debug"]["Increment3"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        controls.controlsByGroup["Debug"]["Decrement3"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        
        break
        
      case CMDS['CMD_DBG_INC4'].address:
      case CMDS['CMD_DBG_DEC4'].address:
        
        // extract data bytes from packet
        var dataBytes = getDataBytes( 'CMD_DBG_INC4' )
        
        // convert to meaningful value 
        var value = bytesToUnsignedLong( dataBytes )
        
        // graphically update value field in control (or graph
        // if we have one set up for this variable)
        if ( typeof graphDebugInc4 !== 'undefined' )
          graphDebugInc4.addPoint( value )
        //else
          controls.controlsByGroup["Debug"]["Increment4"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        controls.controlsByGroup["Debug"]["Decrement4"].querySelector( '#value' ).innerHTML = " (" + value + ")"
        break
        
      default:
        
        // don't recognize this header identifier, assume that we simply
        // do not care about handling this case, i.e. it may be a return
        // for a command we sent that we don't need confirmation for
        //console.log( 'Unrecognized header in received packet: ' + headerByte )
        
    }
  
  }
  
  return
}

function formatArr( arr, cols=1, conv=0 ) {
  
  newArr = []
  if ( conv ) {
    for ( var count=0; count<arr.length; count++ ) {
      // calculate voltage value assuming 10-bit ADC with 3.3V reference
      arr[count] = arr[count] * 3.3 / ((1<<10)-1)
    }
  }
  
  // build up array
  for ( var count=0; count<arr.length; count+=cols )
    newArr.push( [] )
  col = 0
  for ( var count=0; count<arr.length; count++ ) {
    if ( (++col) == cols )
      col = 0
    newArr[Math.floor( count / cols )].push( arr[count] )
  }
  
  return newArr
}
function arrToTsv( arr ) {
  
  // create tsv string with data
  var lineArray = []
  arr.forEach( function ( infoArray, index ) {
      var line = infoArray.join( '\t' )
      lineArray.push( line )
  } )
  var tsvContent = lineArray.join( '\n' )
  
  return tsvContent
}
function stringToClipboard( str ) {
  
  // create dummy input, add it to DOM, select and copy it to clipobaord
  var dummyEle = document.createElement( 'textarea' )
  document.body.appendChild( dummyEle )
  dummyEle.setAttribute( 'id', 'dummyEle' )
  document.getElementById( 'dummyEle' ).value = str  
  dummyEle.select()
  document.execCommand( 'copy' )
  document.body.removeChild( dummyEle )
  
  return
}
function exportCsv( arr ) {
  
  // create csv file with data
  var lineArray = []
  arr.forEach( function ( infoArray, index ) {
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
