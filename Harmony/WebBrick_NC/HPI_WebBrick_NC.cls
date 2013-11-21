VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "HarmonyPlugin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
' ===========================================================
' ===
' === Harmony Plugin Example
' === Version 1.0 - D.K. Mattocks
' === 22 April 2005, iDomus Limited
' === For support visit www.iDomus.co.uk
' ===
' === This is the form for the Plugin example.  It can be one of
' === many forms, or indeed could be deleted altogether should
' === you not require any user interface.
' ===
' === You may extend the functionality of the form as much as you
' === wish using all the traditional VB form controls.
' ===
' ===========================================================

' ===========================================================
' ===
' === frmHarmony.DeleteAll - deletes all the information
' === frmHarmony.Reset - resets all Harmony information
' === frmHarmony.Heartbeat - not used
' === frmHarmony.Register - not used
' === frmHarmony.RegisterDevice - create a device
' === frmHarmony.Raise_Event - calls an event for a device
' === frmHarmony.RegisterEvent - creates an event for a device
' === frmHarmony.RegisterDiscretes - creates a discrete state for a device
' === frmHarmony.RegisterMethod - creates a method for a device
' === frmHarmony.RegisterProperty - creates a property for the plugin
' === frmHarmony.StateChange - notifies Harmony of a state change
' === frmHarmony.PropertyValue - notifies Harmony of a property value
' === frmHarmony.DumpDB - not used
' ===
' ===========================================================

Option Explicit

Dim hInternetSession As Long
Const spare = 1



' ===========================================================
' === This is the main registration call back.
' === Harmony will call this procedure when it is started.
' === The parameter is the main interface form for harmony,
' === meaning that you can later perform calls in your program
' === such as frmHarmony.TurnOn "MyDevice" etc
' ===========================================================

Public Sub HarmonyRegistrationEntryPoint(ByRef frmHarmonyForm As Object)
    hInternetSession = 0
    Set frmHarmony = frmHarmonyForm
    
    frmHarmony.Register WEBBRICK_CTRL
    frmHarmony.RegisterNativeController WEBBRICK_CTRL
    
    Form1.Visible = False
    
    hInternetSession = InternetOpen(WEBBRICK_CTRL, INTERNET_OPEN_TYPE_PRECONFIG, vbNullString, vbNullString, 0)
    Form1.ToLog "InternetOpen " & Err.Number
    
    NewWebbrick
    
    frmHarmony.DumpDB "devices"

End Sub

' ===========================================================
' === This routine gets called when the user clicks on the
' === menu option for your plugin within Harmony
' ===========================================================

Public Sub HarmonyShowPlugin()
Form1.Visible = True
End Sub

' ===========================================================
' === This routine gets called when the user clicks on the
' === menu option for your plugin within Harmony
' ===========================================================
Public Sub NewDevice(nodeIp As String, deviceName As String, adr As String)
    ' Get webbrick name
    With frmHarmony
        .RegisterDevice WEBBRICK_CTRL, deviceName, "LED_ON.png"
'        .SetDeviceZone deviceName, nodeName
        .SetDeviceAddress deviceName, nodeIp & ";" & adr
        .SetDeviceType deviceName, "Appliance Module (wired)"
    End With
    
End Sub

Public Sub NewAnOut(nodeIp As String, chn As Integer)
    NewDevice nodeIp, "Analogue " & chn, "A" & chn
End Sub

Public Sub NewDigOut(nodeIp As String, chn As Integer)
    NewDevice nodeIp, "Digital " & chn, "D" & chn
End Sub

Public Sub NewEvent(deviceName As String, eventName As String)
    With frmHarmony
        .RegisterEvent WEBBRICK_CTRL, deviceName, eventName
    End With
    
End Sub

Public Sub NewWebbrick()
    ' Get webbrick name
    Dim nodeName As String
    Dim nodeIp As String
    nodeName = "WebBrick1"
    nodeIp = "192.168.1.99"

    With frmHarmony
        .CreateZone nodeName
        Dim idx As Integer
        
        For idx = 1 To 8
            NewDigOut nodeIp, idx
        Next
        
        For idx = 1 To 4
            NewAnOut nodeIp, idx
        Next
        
        NewDevice nodeIp, "DigitalInputs", ""
        For idx = 1 To 8
            frmHarmony.RegisterEvent WEBBRICK_CTRL, "DigitalInputs", "DigTrigger " & idx
        Next
        
    End With
    
End Sub


' ===========================================================
'
'   Send a command to a remote webbrick.
'
'
'

Public Sub Send(host As String, cmd As String)
    On Error Resume Next
    Dim request As String
    Dim hConnection As Long
    hConnection = 0
    
    Err.Number = 0
    
    request = "http://" & host & "/cfg.spi?com=" & cmd
    Form1.ToLog request
    
    hConnection = InternetOpenUrl(hInternetSession, request, vbNull, 0, INTERNET_FLAG_NO_AUTO_REDIRECT, vbNull)
    If (hConnection <> 0) Then
        Dim strBuffer As String * 128
        Dim readLen As Long
        While (InternetReadFile(hConnection, strBuffer, 128, readLen) And (readLen > 0))
            Form1.ToLog strBuffer
        Wend
    Else
        Form1.ToLog "InternetOpenUrl " & Err.Number
    End If
    
    If hConnection <> 0 Then InternetCloseHandle (hConnection)
End Sub

' ===========================================================
' === This routine is called frequently.  It will be called
' === whenever anything of interest happens within Harmony
' === such as a device being switched, a system event
' === occurring, startup, shutdown etc
' ===========================================================

Public Sub HarmonyNotify(ByVal sEvent As String)
    Debug.Print "Event notification received : " & sEvent
    Form1.ToLog sEvent
    DoEvents
End Sub

' ===========================================================
' === This callback is called whenever the system wants to
' === call a function / sub in your plugin
' ===========================================================
        
Public Sub HarmonyCallMethod(ByVal sDeviceName As String, ByVal sMethod As String, ByVal sParams As String)
    Dim cmd As String
    Dim chnType As String
    Dim chnNr As String
    Dim host As String
    Dim fade As String
    Dim params
    Dim params2
    Dim param
    
    Form1.ToLog sDeviceName & "," & sMethod & "," & sParams
    params = Split(sParams, "|")
    For Each param In params
        Form1.ToLog "1-" & param
    Next
    params2 = Split(params(1), ";")
    For Each param In params
        Form1.ToLog "2-" & param
    Next
    host = params2(0)
    
    chnType = Left(params2(1), 1)
    chnNr = val(Mid(params2(1), 2)) - 1
    
    Form1.ToLog host & ":" & chnType & ":" & chnNr
    
    If (chnType = "D") Then
        cmd = "DO" & chnNr & ";"
        Select Case sMethod
            Case "TurnOn":
                Send host, cmd & "N:"
            Case "TurnOff":
                Send host, cmd & "F:"
            Case "On":
                Send host, cmd & "N:"
            Case "Off":
                Send host, cmd & "F:"
        End Select
    Else
        cmd = "AA" & chnNr & ";"
        Select Case sMethod
            Case "TurnOn":
                Send host, cmd & "100:"
            Case "TurnOff":
                Send host, cmd & "0:"
            Case "On":
                Send host, cmd & "100:"
            Case "Off":
                Send host, cmd & "0:"
            Case "FadeTo":
                params2 = Split(params(0), ",")
                fade = params2(0)
                Form1.ToLog fade
                Send host, cmd & Mid(params(0), 2) & ":"
        End Select
    End If
End Sub
        
' ===========================================================
' === This callback is called when Harmony want you to
' === identify your plugin
' ===========================================================
        
Public Sub HarmonyIdent(ByVal Param1 As String)

End Sub
                
' ===========================================================
' === This callback is called by Harmony when it wants to
' === refresh its knowledge about the state of the
' === "Properties" within your plugin
' ===========================================================
        
Public Sub HarmonyRefreshProperties(ByVal Param1 As String)

End Sub
                
' ===========================================================
' === This callback is called by the system to trigger an
' === action that you have defined as a discrete trigger
' === action
' ===========================================================
        
Public Sub HarmonyTriggerDiscrete(ByVal Param1 As String)

End Sub

' ===========================================================
' === This routine is here for completeness only
' === as it hardly ever gets called.  It should be provided
' === so that Harmony can terminate your program but 9 times
' === out of 10 Harmony will simply terminate itself
' === and then let your prgram be closed by the Windows
' === operating system naturally.
' ===========================================================

Public Sub HarmonyTerminatePlugin()
    If hInternetSession <> 0 Then InternetCloseHandle (hInternetSession)

End Sub
' ===========================================================
' === End
' ===========================================================

