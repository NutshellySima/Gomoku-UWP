/*
   Copyright 2017 Chijun Sima

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Threading;
using Windows.UI;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.Resources;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Input;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using unmanaged;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace gomoku_uwp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Home : Page
    {
        class chessboard_line_class
        {
            public UIElement line;
            public int i;
            public bool horizontal;
            public chessboard_line_class(UIElement line, int i, bool horizontal)
            {
                this.line = line;
                this.i = i;
                this.horizontal = horizontal;
            }
        }
        class chessboard_point_class
        {
            public UIElement point;
            public int row, col;
            public chessboard_point_class(UIElement point, int row, int col)
            {
                this.point = point;
                this.row = row;
                this.col = col;
            }
        }
        class chessboard_notice_line_class
        {
            public UIElement notice_line;
            public int row, col;
            public bool first;
            public chessboard_notice_line_class(UIElement notice_line, int row, int col, bool first)
            {
                this.notice_line = notice_line;
                this.row = row;
                this.col = col;
                this.first = first;
            }
        }
        // Global definitions
        ResourceLoader rl = new ResourceLoader();
        SemaphoreSlim chessboard_lock = new SemaphoreSlim(1);
        bool operationCanceled = false;
        private int turn = 1;
        string flyout_mode;
        string flyout_black;
        string flyout_white;
        // Resource
        Windows.Storage.ApplicationDataContainer _gameOptions = Windows.Storage.ApplicationData.Current.LocalSettings;
        private unmanaged.cliwrapper invoke = new unmanaged.cliwrapper();
        // UIElement List
        private List<chessboard_line_class> chessboard_line = new List<chessboard_line_class>();
        private List<chessboard_point_class> chessboard_point = new List<chessboard_point_class>();
        private List<chessboard_notice_line_class> chessboard_noticeline = new List<chessboard_notice_line_class>();
        public Home()
        {
            this.InitializeComponent();
            chessboard_lock.Wait();
            initSettings();
            InitWindow();
            chessboard_lock.Release();
        }
        public void initSettings()
        {
            Object valueMode = _gameOptions.Values["mode"];
            Object valueBlack = _gameOptions.Values["black"];
            Object valueWhite = _gameOptions.Values["white"];
            if (valueMode == null || valueBlack == null || valueWhite == null)
            {
                _gameOptions.Values["mode"] = "hard";
                _gameOptions.Values["black"] = "computer";
                _gameOptions.Values["white"] = "human";
            }
            else
            {
                switch (_gameOptions.Values["mode"])
                {
                    case "hard":
                        Mode_Hard.IsChecked = true;
                        break;
                    case "easy":
                        Mode_Easy.IsChecked = true;
                        break;
                }
                switch (_gameOptions.Values["black"])
                {
                    case "computer":
                        Black_Computer.IsChecked = true;
                        break;
                    case "human":
                        Black_Human.IsChecked = true;
                        break;
                }
                switch (_gameOptions.Values["white"])
                {
                    case "computer":
                        White_Computer.IsChecked = true;
                        break;
                    case "human":
                        White_Human.IsChecked = true;
                        break;
                }
            }
        }
        private void B_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton rb = sender as RadioButton;
            if (rb != null)
            {
                string Name = rb.Tag.ToString();
                switch (Name)
                {
                    case "Hard":
                        _gameOptions.Values["mode"] = "hard";
                        break;
                    case "Easy":
                        _gameOptions.Values["mode"] = "easy";
                        break;
                    case "ComputerWhite":
                        _gameOptions.Values["white"] = "computer";
                        break;
                    case "HumanWhite":
                        _gameOptions.Values["white"] = "human";
                        break;
                    case "ComputerBlack":
                        _gameOptions.Values["black"] = "computer";
                        break;
                    case "HumanBlack":
                        _gameOptions.Values["black"] = "human";
                        break;
                }
            }
        }
        // specific Draw related
        public void Drawchessboard(int i, bool horizontal)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Line line;
            line = new Windows.UI.Xaml.Shapes.Line();
            if (horizontal == true)
            {
                line.X1 = deltax;
                line.X2 = 15 * deltax;
                line.Y1 = deltay * i;
                line.Y2 = deltay * i;
            }
            else
            {
                line.Y1 = deltay;
                line.Y2 = 15 * deltay;
                line.X1 = deltax * i;
                line.X2 = deltax * i;
            }
            line.X1 += 10;
            line.X2 += 10;
            line.Y1 += 10;
            line.Y2 += 10;
            line.IsHitTestVisible = false;
            line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 2.5;
            chessboard_father.Children.Add(line);
            chessboard_line.Add(new chessboard_line_class(line, i, horizontal));
        }
        public void DrawPoint(int row, int col, bool black)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Ellipse circle;
            circle = new Windows.UI.Xaml.Shapes.Ellipse()
            {
                Height = 0.75 * deltay,
                Width = 0.75 * deltax
            };
            double left = (row - 7) * 2 * deltax;
            double top = (col - 7) * 2 * deltay;
            circle.Margin = new Thickness(left, top, 0, 0);
            if (black == true)
                circle.Fill = new SolidColorBrush(Colors.Black);
            else
                circle.Fill = new SolidColorBrush(Colors.White);
            chessboard_father.Children.Add(circle);
            chessboard_point.Add(new chessboard_point_class(circle, row, col));
        }
        public void DrawNoticeLine(int row, int col, bool black)
        {
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            Windows.UI.Xaml.Shapes.Line line;
            line = new Windows.UI.Xaml.Shapes.Line()
            {
                X1 = 10 + (row + 0.875) * deltax,
                X2 = 10 + (row + 0.875) * deltax + 0.25 * deltax,
                Y1 = 10 + (col + 1) * deltay,
                Y2 = 10 + (col + 1) * deltay
            };
            if (black)
                line.Stroke = new SolidColorBrush(Colors.White);
            else
                line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 3;
            chessboard_father.Children.Add(line);
            chessboard_noticeline.Add(new chessboard_notice_line_class(line, row, col, true));
            line = new Windows.UI.Xaml.Shapes.Line()
            {
                Y1 = 10 + (col + 0.875) * deltay,
                Y2 = 10 + (col + 0.875) * deltay + 0.25 * deltay,
                X1 = 10 + (row + 1) * deltax,
                X2 = 10 + (row + 1) * deltax
            };
            if (black)
                line.Stroke = new SolidColorBrush(Colors.White);
            else
                line.Stroke = new SolidColorBrush(Colors.Black);
            line.StrokeThickness = 3;
            chessboard_father.Children.Add(line);
            chessboard_noticeline.Add(new chessboard_notice_line_class(line, row, col, false));
        }
        // specific Remove related
        public void Remove_noticeLine(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_noticeline[chessboard_noticeline.Count - 1].notice_line);
                chessboard_noticeline.RemoveAt(chessboard_noticeline.Count - 1);
            }
        }
        public void Remove_chessboardLine(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_line[chessboard_line.Count - 1].line);
                chessboard_line.RemoveAt(chessboard_line.Count - 1);
            }
        }
        public void Remove_chessboardPoint(int i)
        {
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_point[chessboard_point.Count - 1].point);
                chessboard_point.RemoveAt(chessboard_point.Count - 1);
            }
        }
        // specific Clear related
        public void Clear_noticeLine()
        {
            var i = chessboard_noticeline.Count;
            for (int x = 0; x < i; ++x)
            {
                chessboard_father.Children.Remove(chessboard_noticeline[chessboard_noticeline.Count - 1].notice_line);
                chessboard_noticeline.RemoveAt(chessboard_noticeline.Count - 1);
            }
        }
        // Main logic related
        public async void InitWindow()
        {
            for (int ii = 1; ii <= 15; ++ii)
            {
                Drawchessboard(ii, true);
                Drawchessboard(ii, false);
            }
            await PlayGame();
        }
        public void WinPrinter()
        {
            Clear_noticeLine();
            UndoButton.IsEnabled = false;
            RestartButton.IsEnabled = false;
            Settings_nav.IsEnabled = false;
            invoke.Checkwin(true);
            var board = invoke.Getchessboard();
            for (int ii = 0; ii < 15; ++ii)
            {
                for (int iii = 0; iii < 15; ++iii)
                {
                    if (board[15 * ii + iii] == 3)
                        DrawNoticeLine(iii, ii, true);
                    if (board[15 * ii + iii] == 4)
                        DrawNoticeLine(iii, ii, false);
                }
            }
            invoke.Checkwin(false);
            UndoButton.IsEnabled = true;
            RestartButton.IsEnabled = true;
            Settings_nav.IsEnabled = true;
        }
        private async Task PlaybyComputer(bool black)
        {
            UndoButton.IsEnabled = false;
            RestartButton.IsEnabled = false;
            Settings_nav.IsEnabled = false;
            String original = nav_bar.Text;
            nav_bar.Text = rl.GetString("thinking_txt");
            Task<int[]> com;
            if (black)
            {
                if (_gameOptions.Values["mode"].ToString() == "hard")
                    com = Task.Run(() => invoke.Putcomputer(true, 1));
                else
                    com = Task.Run(() => invoke.Putcomputer(false, 1));
            }
            else
            {
                if (_gameOptions.Values["mode"].ToString() == "hard")
                    com = Task.Run(() => invoke.Putcomputer(true, 2));
                else
                    com = Task.Run(() => invoke.Putcomputer(false, 2));
            }
            await com;
            var data = com.Result;
            if (data[2] == -1 || data[1] == -1)
            {
                UndoButton.IsEnabled = true;
                RestartButton.IsEnabled = true;
                Settings_nav.IsEnabled = true;
                nav_bar.Text = original;
                Update_Output();
                return;
            }
            Clear_noticeLine();
            if (black)
            {
                DrawPoint(data[2], data[1], true);
                DrawNoticeLine(data[2], data[1], true);
            }
            else
            {
                DrawPoint(data[2], data[1], false);
                DrawNoticeLine(data[2], data[1], false);
            }
            ++turn;
            UndoButton.IsEnabled = true;
            RestartButton.IsEnabled = true;
            Settings_nav.IsEnabled = true;
            Update_Output();
            if (black && invoke.Checkwin(false) == 1)
            {
                WinPrinter();
                ContentDialog Result = new ContentDialog
                {
                    Title = rl.GetString("Result_txt"),
                    Content = rl.GetString("win_bc"),
                    CloseButtonText = rl.GetString("ok_txt"),
                    CloseButtonStyle = this.Resources["b_s"] as Style
                };
                ContentDialogResult result = await Result.ShowAsync();
            }
            else if ((!black) && invoke.Checkwin(false) == 2)
            {
                WinPrinter();
                ContentDialog Result = new ContentDialog
                {
                    Title = rl.GetString("Result_txt"),
                    Content = rl.GetString("win_wc"),
                    CloseButtonText = rl.GetString("ok_txt"),
                    CloseButtonStyle = this.Resources["b_s"] as Style
                };
                ContentDialogResult result = await Result.ShowAsync();
            }
            await PlayGame();
        }
        public async Task PlayGame(bool computer = true)
        {
            if (turn == 1 || (turn == 2 && _gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "human") || (_gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "computer"))
            {
                UndoButton.IsEnabled = false;
            }
            else
            {
                UndoButton.IsEnabled = true;
            }
            if (operationCanceled)
                return;
            if (invoke.Fullboard() && invoke.Checkwin(false) == 0)
            {
                ContentDialog Fullboard = new ContentDialog
                {
                    Title = rl.GetString("fullboard"),
                    Content = rl.GetString("fullboard_txt"),
                    CloseButtonText = rl.GetString("ok_txt"),
                    CloseButtonStyle = this.Resources["b_s"] as Style
                };
                ContentDialogResult result = await Fullboard.ShowAsync();
                return;
            }
            if (invoke.Checkwin(false) == 0)
            {
                //Black
                if (turn % 2 == 1)
                {
                    if (_gameOptions.Values["black"].ToString() == "computer" && computer == true)
                    {
                        await PlaybyComputer(true);
                    }
                    else
                    {
                        nav_bar.Text = rl.GetString("your_turn_txt");
                    }
                }
                //White
                else
                {
                    if (_gameOptions.Values["white"].ToString() == "computer" && computer == true)
                    {
                        await PlaybyComputer(false);
                    }
                    else
                    {
                        nav_bar.Text = rl.GetString("your_turn_txt");
                    }
                }
            }
            else
            {
                nav_bar.Text = rl.GetString("game_over_txt");
                return;
            }
        }
        public async Task<bool> UndoGame()
        {
            bool Undostatus = invoke.BoardUndo();
            if (Undostatus == false)
            {
                await PlayGame(false);
                return false;
            }
            else
            {
                Clear_noticeLine();
                Remove_chessboardPoint(1);
                --turn;
                Update_Output();
                if (turn == 1 || (turn == 2 && _gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "human") || (_gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "computer"))
                    UndoButton.IsEnabled = false;
                if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 3)
                {
                    var x3 = invoke.Gethistory()[invoke.Gethistory().Length - 1];
                    var x2 = invoke.Gethistory()[invoke.Gethistory().Length - 2];
                    var x1 = invoke.Gethistory()[invoke.Gethistory().Length - 3];
                    if (x1 == 1)
                        DrawNoticeLine(x3, x2, true);
                    else
                        DrawNoticeLine(x3, x2, false);
                }
                await PlayGame(false);
                return true;
            }
        }
        private async void chessboard_background_PointerReleased(object sender, PointerRoutedEventArgs e)
        {
            bool valid_Val = await chessboard_lock.WaitAsync(0);
            if (!valid_Val)
                return;
            // locked
            if (invoke.Checkwin(false) != 0)
            {
                chessboard_lock.Release();
                return;
            }
            var deltax = Convert.ToDouble(chessboard_background.ActualWidth) / 16;
            var deltay = Convert.ToDouble(chessboard_background.ActualHeight) / 16;
            PointerPoint ptr = e.GetCurrentPoint(chessboard_background);
            Point pt = ptr.Position;
            if (turn % 2 == 1)
            {
                bool x = invoke.Puthuman((int)Math.Round(pt.Y / deltay) - 1, (int)Math.Round(pt.X / deltax) - 1, 1);
                if (x == true)
                {
                    Clear_noticeLine();
                    DrawPoint((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, true);
                    DrawNoticeLine((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, true);
                    ++turn;
                    Update_Output();
                    UndoButton.IsEnabled = true;
                    await PlayGame();
                    if (invoke.Checkwin(false) == 1)
                    {
                        WinPrinter();
                        ContentDialog Result = new ContentDialog
                        {
                            Title = rl.GetString("Result_txt"),
                            Content = rl.GetString("win_bh"),
                            CloseButtonText = rl.GetString("ok_txt"),
                            CloseButtonStyle = this.Resources["b_s"] as Style
                        };
                        ContentDialogResult result = await Result.ShowAsync();
                    }
                }
                else
                {
                    chessboard_lock.Release();
                    return;
                }
            }
            else
            {
                bool x = invoke.Puthuman((int)Math.Round(pt.Y / deltay) - 1, (int)Math.Round(pt.X / deltax) - 1, 2);
                if (x == true)
                {
                    Clear_noticeLine();
                    DrawPoint((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, false);
                    DrawNoticeLine((int)Math.Round(pt.X / deltax) - 1, (int)Math.Round(pt.Y / deltay) - 1, false);
                    ++turn;
                    Update_Output();
                    UndoButton.IsEnabled = true;
                    await PlayGame();
                    if (invoke.Checkwin(false) == 2)
                    {
                        WinPrinter();
                        ContentDialog Result = new ContentDialog
                        {
                            Title = rl.GetString("Result_txt"),
                            Content = rl.GetString("win_wh"),
                            CloseButtonText = rl.GetString("ok_txt"),
                            CloseButtonStyle = this.Resources["b_s"] as Style
                        };
                        ContentDialogResult result = await Result.ShowAsync();
                    }
                }
                else
                {
                    chessboard_lock.Release();
                    return;
                }
            }
            chessboard_lock.Release();
        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            operationCanceled = true;
        }

        private async void UndoButton_Click(object sender, RoutedEventArgs e)
        {
            if (_gameOptions.Values["black"].ToString() == "computer" && _gameOptions.Values["white"].ToString() == "computer")
                return;
            await chessboard_lock.WaitAsync();
            if (_gameOptions.Values["black"].ToString() == "human" && _gameOptions.Values["white"].ToString() == "human")
            {
                if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 3)
                    Clear_noticeLine();
                bool Undostatus = await UndoGame();
            }
            else
            {
                if (_gameOptions.Values["black"].ToString() == "human" && _gameOptions.Values["white"].ToString() == "computer" && invoke.Checkwin(false) == 1)
                {
                    Clear_noticeLine();
                    await UndoGame();
                }
                else if (invoke.Gethistory() != null && invoke.Gethistory().Length >= 6)
                {
                    Clear_noticeLine();
                    await UndoGame();
                    await UndoGame();
                }
            }
            chessboard_lock.Release();
        }

        private async void RestartButton_Click(object sender, RoutedEventArgs e)
        {
            await chessboard_lock.WaitAsync();
            Clear_noticeLine();
            while (await UndoGame() == true) ;
            await PlayGame();
            chessboard_lock.Release();
        }

        private void Update_Output()
        {
            char a1, a2;
            var history = invoke.Gethistory();
            string output = "";
            var length = 0;
            if (history != null)
                length = history.Length;
            output += rl.GetString("turn_txt") + Convert.ToString(turn - 1);
            if (length != 0)
                output += "\n";
            for (int ii = 0; ii < length / 3; ++ii)
            {
                a1 = (char)(history[ii * 3 + 1] + 'A');
                a2 = (char)(history[ii * 3 + 2] + 'A');
                output += Convert.ToString(ii + 1).PadLeft(3) + ": " + Convert.ToString(a1) + Convert.ToString(a2) + " ";
                if (ii != 0 && (ii + 1) % 6 == 0 && ii != length / 3 - 1)
                    output += "\n";
            }
            Dump_Text.Text = output;
        }

        private void DumpButton_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private async void Flyout_Closed(object sender, object e)
        {
            var currentSettings = Windows.Storage.ApplicationData.Current.LocalSettings;
            if ((currentSettings.Values["mode"].ToString() == flyout_mode) && (currentSettings.Values["black"].ToString() == flyout_black) && (currentSettings.Values["white"].ToString() == flyout_white))
                return;
            if ((currentSettings.Values["black"].ToString() == flyout_black) && (currentSettings.Values["white"].ToString() == flyout_white) && flyout_black == "human" && flyout_white == "human")
                return;
            await chessboard_lock.WaitAsync();
            Clear_noticeLine();
            while (await UndoGame() == true) ;
            await PlayGame();
            chessboard_lock.Release();
        }

        private async void HyperlinkButton_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog about = new AboutDialog();
            await about.ShowAsync();
        }

        private void Flyout_Opened(object sender, object e)
        {
            initSettings();
        }

        private void Flyout_Opening(object sender, object e)
        {
            initSettings();
            Windows.Storage.ApplicationDataContainer temp = Windows.Storage.ApplicationData.Current.LocalSettings;
            flyout_mode = temp.Values["mode"].ToString();
            flyout_black = temp.Values["black"].ToString();
            flyout_white = temp.Values["white"].ToString();
        }
    }
}
