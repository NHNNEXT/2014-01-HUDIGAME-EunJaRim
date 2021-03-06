﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestClient1
{
    public partial class Form1 : Form
    {
        bool    IsServerConnected = false;
        int playerID = -1;
        
        public Form1()
        {
            InitializeComponent();
            var rnd = new Random(DateTime.Now.Millisecond);
            playerID = rnd.Next(101, 199);
            lb_playerID.Text = "PlayerID:" + playerID;
        }


        private void bu_connect_server_Click(object sender, EventArgs e)
        {
            if (Network.ConnectServer())
            {
                IsServerConnected = true;
                lb_connect_server.Text = "CONNECTED!";
                Network.SendLoginRequest(playerID);
            }
            else
            {
                lb_connect_server.Text = "CONNECT ERROR!";
            }
        }


        private void bu_gameOver_send_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                Network.SendGameOverRequest(int.Parse(tb_gameOver_playerId.Text), cb_gameOver_isWin.Checked);
            }
        }

        private void bu_roomCreate_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                Network.SendRoomCreateRequest(playerID);
            }
        }

        private void bu_roomEnter_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                Network.SendEnterRoomRequest(int.Parse(tb_roomEnterNum.Text));
            }
        }

        private void bu_roomLeave_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                Network.SendLeaveRoomRequest(int.Parse(tb_roomLeaveNum.Text));
            }
        }

        private void bu_generate_corps_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                byte unitType = 0;
                if (rb_unit_arrow.Checked)
                {
                    unitType = (byte)UnitType.UNIT_ARROW;
                }
                else if (rb_unit_guard.Checked)
                {
                    unitType = (byte)UnitType.UNIT_GUARD;
                }
                else if (rb_unit_knight.Checked)
                {
                    unitType = (byte)UnitType.UNIT_KNIGHT;
                }
                else if (rb_unit_pike.Checked)
                {
                    unitType = (byte)UnitType.UNIT_PIKE;
                }
                else if (rb_unit_sword.Checked)
                {
                    unitType = (byte)UnitType.UNIT_SWORD;
                }

                float eyeX = float.Parse(tb_corps_eye_x.Text);
                float eyeZ = float.Parse(tb_corps_eye_z.Text);

                float lookX = float.Parse(tb_corps_look_x.Text);
                float lookZ = float.Parse(tb_corps_look_z.Text);

                Network.SendGenerateCorpsRequest(unitType, eyeX, eyeZ, lookX,lookZ, playerID);
            }
        }

        private void bu_corps_move_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                int corpsID = int.Parse(tb_corpsID.Text);
                float eyeX = float.Parse(tb_corps_eye_x.Text);
                float eyeZ = float.Parse(tb_corps_eye_z.Text);

                float lookX = float.Parse(tb_corps_look_x.Text);
                float lookZ = float.Parse(tb_corps_look_z.Text);

                Network.SendMoveCorpsRequest(corpsID, eyeX, eyeZ, lookX, lookZ);
            }
        }

        private void bu_corps_stop_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                int corpsID = int.Parse(tb_corpsID.Text);
                Network.SendStopCorpsRequest(corpsID);
            }
        }

        private void bu_corps_formation_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                byte formationType = 0;
                if (rb_corps_formation_defense.Checked)
                {
                    formationType = (byte)FormationType.FORMATION_DEFENSE;
                }
                else if (rb_corps_formation_destroy.Checked)
                {
                    formationType = (byte)FormationType.FORMATION_DESTROY;
                }
                else if (rb_corps_formation_rush.Checked)
                {
                    formationType = (byte)FormationType.FORMATION_RUSH;
                }

                int corpsID = int.Parse(tb_corpsID.Text);
                Network.SendChangeCorpsFormationRequest(corpsID, formationType);
            }
        }

        private void bu_Attack_Corps_Click(object sender, EventArgs e)
        {
            if (IsServerConnected)
            {
                int corpsIDFrom = int.Parse(tb_Attack_from_corpsID.Text);
                int corpsIDTo = int.Parse(tb_Attack_to_corpsID.Text);
                Network.AttackCorpsRequest(corpsIDFrom, corpsIDTo);

            }
        }

    }
}
